#include <JXRTest.h>
#include <QDataStream>
#include "hdpconverter.hpp"

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

ERR HdpConverter::PKCodecFactory_CreateDecoderFromMemory(
        const QByteArray &source,
        PKImageDecode** ppDecoder
        )
{
    ERR err = WMP_errSuccess;

    const PKIID *pIID = nullptr;

    struct WMPStream *pStream = nullptr;
    PKImageDecode *pDecoder = nullptr;

    // get decode PKIID
    Call(GetImageDecodeIID(".hdp", &pIID));

    // create stream
    Call(CreateWS_Memory(&pStream, static_cast<void*>(const_cast<char*>(source.data())), static_cast<size_t>(source.size())));

    // Create decoder
    Call(PKCodecFactory_CreateCodec(pIID, reinterpret_cast<void**>(ppDecoder)));
    pDecoder = *ppDecoder;

    // attach stream to decoder
    Call(pDecoder->Initialize(pDecoder, pStream)); // 이 부분에서 이미지 데이터의 내용을 분석한다.
    pDecoder->fStreamOwner = !0;

Cleanup:
    return err;
}

ERR HdpConverter::WriteQImageHeader(
        PKImageEncode *pIE,
        QImage &image
        )
{
    //JXR 라이브러리에서 사용하는 해상도 단위는 인치. QT에서 쓰는건 미터단위.
    image.setDotsPerMeterX(inchConvertToMeter(pIE->fResX));
    image.setDotsPerMeterY(inchConvertToMeter(pIE->fResY));

    //헤더 기록 완료
    pIE->fHeaderDone = !FALSE;

    return WMP_errSuccess;
}

ERR PKImageEncode_WritePixels_QImage(
        PKImageEncode *pIE,
        U32 cLine,
        U8 *pbPixel,
        U32 cbStride
        )
{
    ERR err = WMP_errSuccess;

    QImage::Format format;
    size_t cbLineM = 0;
    QByteArray *data = nullptr;
    int size = pIE->uWidth * pIE->uHeight * 4;

    // < -- header -- >
    if ( ! pIE->fHeaderDone ) {
        HdpConverter::singleton->WriteQImageHeader(pIE, HdpConverter::singleton->image);
    }

    // calculate line size in memory and in stream
    cbLineM = pIE->cbPixel * pIE->uWidth;

    FailIf(cbStride < cbLineM, WMP_errInvalidParameter);

    if ( pIE->WMP.bHasAlpha ) {
        format = QImage::Format_ARGB32_Premultiplied;
        data = HdpConverter::convertRgbaToArgb(pbPixel, size);
    }
    else {
        format = QImage::Format_RGB888;
        data = &HdpConverter::singleton->imageRawData;
        data->setRawData(reinterpret_cast<char*>(pbPixel), size);
    }

    HdpConverter::singleton->image = QImage(
                reinterpret_cast<uchar*>(data->data()),
                pIE->uWidth,
                pIE->uHeight,
                cbStride,
                format
                );
    pIE->idxCurrentLine += cLine;

Cleanup:
    return err;
}

HdpConverter *HdpConverter::singleton = nullptr;

/** 인스턴스를 얻습니다.\n
  만약 인스턴스가 생성되어 있지 않다면 자동으로 생성합니다.
  @return HdpConverter 인스턴스.
  */
HdpConverter* HdpConverter::getInstance()
{
    if ( singleton == nullptr ) {
        singleton = new HdpConverter();
    }

    return singleton;
}

/** 생성자.
  */
HdpConverter::HdpConverter()
{
    // < -- 인스턴스 설정 -- >
    singleton = this;
}

/** RGBA 순서의 바이트 배열을 ARGB 순서로 재배열한다.
  @return 데이터 포인터를 반환한다. delete하지 말것.
  */
QByteArray* HdpConverter::convertRgbaToArgb(
        uchar *rgbaData, ///< 재배열 할 데이터
        uint size        ///< 바이트 수
        )
{
    QDataStream rgba;
    QDataStream argb(&imageRawData, QIODevice::WriteOnly);

    rgba.writeRawData(reinterpret_cast<char*>(rgbaData), size);

    quint8 a = 0, rgb[3];
    for(uint i = 0; i < size; i += 4) {
        rgba.readRawData(reinterpret_cast<char*>(rgb), 3);
        rgba.readRawData(reinterpret_cast<char*>(a), 1);
        argb.writeRawData(reinterpret_cast<char*>(a), 1);
        argb.writeRawData(reinterpret_cast<char*>(rgb), 3);
    }

    return &imageRawData;
}

/** HdpConverter를 할당 해제합니다.
  */
void HdpConverter::relrease()
{
    delete this;
}

/** 소멸자.
  */
HdpConverter::~HdpConverter()
{
    singleton = nullptr;
}

/** HDP 포멧 형식의 파일 데이터를 읽어옵니다.
  @return HdpConverter 인스턴스.
  @throw 오류가 존재할 경우 WMP_err를 던집니다. 오류를 받기 위해서는 wmp_err.hpp 헤더 파일이 필요합니다. Commit or rollback semantics.
  */
HdpConverter* HdpConverter::setData(
        const QByteArray &hdpData
        )
{
    ERR err = WMP_errSuccess;

    PKImageDecode *pDecoder = nullptr;
    PKFactory *pFactory = nullptr;
    PKCodecFactory *pCodecFactory = nullptr;
    WMPStream *pEncodeStream = nullptr; // 인코드 스트림
    PKImageEncode *pEncoder = nullptr; // 인코더
    PKFormatConverter *pConverter = nullptr; // 컨버터
    PKPixelFormatGUID guidPixFormat;
    PKRect rect = {0, 0, 0, 0}; // 크기

    Call(PKCreateFactory(&pFactory, PK_SDK_VERSION));
    Call(PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION));
    Call(PKCodecFactory_CreateDecoderFromMemory(hdpData, &pDecoder));

    pDecoder->WMP.bIgnoreOverlap = FALSE;

    // < -- Validate thumbnail decode parameters -- >
    pDecoder->WMP.wmiI.cThumbnailWidth = pDecoder->WMP.wmiI.cWidth;
    pDecoder->WMP.wmiI.cThumbnailHeight = pDecoder->WMP.wmiI.cHeight;
    pDecoder->WMP.wmiI.bSkipFlexbits = FALSE;

    // < -- 디코딩 영역 지정 -- >
    pDecoder->WMP.wmiI.cROILeftX = 0; // 전체 디코딩
    pDecoder->WMP.wmiI.cROITopY = 0;
    pDecoder->WMP.wmiI.cROIWidth = pDecoder->WMP.wmiI.cThumbnailWidth;
    pDecoder->WMP.wmiI.cROIHeight = pDecoder->WMP.wmiI.cThumbnailHeight;

    // < -- 이미지 방향 설정 -- >
    pDecoder->WMP.wmiI.oOrientation = O_NONE; // 원본 그대로 유지

    // < -- 이미지 전처리 설정 -- >
    pDecoder->WMP.wmiI.cPostProcStrength = 0; // 전처리 없음

    // < -- 작업 현황 보고 설정 -- >
    pDecoder->WMP.wmiSCP.bVerbose = FALSE; // 보고 안함

    // < -- 알파값 유무에 따른 포멧 선택 -- >
    if ( pDecoder->WMP.bHasAlpha ) {
        guidPixFormat = GUID_PKPixelFormat32bppRGBA;
    }
    else {
        guidPixFormat = GUID_PKPixelFormat32bppRGB;
    }

    // < -- 컨버터 초기화 -- >
    Call(pCodecFactory->CreateFormatConverter(&pConverter));
    Call(pConverter->Initialize(pConverter, pDecoder, nullptr, guidPixFormat)); // 널 포인터를 인자로 주어 확장자에 따른 픽셀 포멧 지정을 막음.

    // < -- 인코더 준비 -- >
    Call(pFactory->CreateStreamFromMemory(&pEncodeStream, nullptr, 0)); //의미 없음. 함수 포인터에 저절한 더미 함수를 배치 하기 위한 작업.
    Call(PKImageEncode_Create(&pEncoder));
    pEncoder->WritePixels = PKImageEncode_WritePixels_QImage;
    pEncoder->Initialize(pEncoder, pEncodeStream, nullptr, 0);

    // < -- 출력 화소 포멧 설정 -- >
    Call(pEncoder->SetPixelFormat(pEncoder, guidPixFormat));
    pEncoder->WMP.wmiSCP.bBlackWhite = pDecoder->WMP.wmiSCP.bBlackWhite;

    // < -- 출력 이미지 크기 설정 -- >
    rect.Width = static_cast<I32>(pDecoder->WMP.wmiI.cROIWidth);
    rect.Height = static_cast<I32>(pDecoder->WMP.wmiI.cROIHeight);
    Call(pEncoder->SetSize(pEncoder, rect.Width, rect.Height));

    // < -- 출력 해상도 설정 -- >
    {
        Float rX = 0.0, rY = 0.0; // 해상도
        Call(pDecoder->GetResolution(pDecoder, &rX, &rY)); // 디코더의 해상도를 가져옵니다.
        Call(pEncoder->SetResolution(pEncoder, rX, rY)); // 인코더의 해상도를 디코더의 해상도로 설정합니다.
    }

    // < -- 변환된 내용을 기록 -- >
    pEncoder->WriteSource = PKImageEncode_Transcode;
    Call(pEncoder->WriteSource(pEncoder, pConverter, &rect));

    // < -- 인코더 할당 해제 -- >
    pEncoder->Release(&pEncoder);

    // < -- 디코더 할당 해제 -- >
    pDecoder->Release(&pDecoder);

Cleanup:
    if ( Failed(err) ) {
        throw WMP_err(err);
    }

    return this;
}

/** 알파 채널이 포함되었는지 확인한다.
  @return 알파값 포함 여부. true : 포함됨. false : 없음.
  */
bool HdpConverter::hasAlphaChannel()
{
    return image.hasAlphaChannel();
}

bool HdpConverter::saveToJpeg(
        const QString &filePath ///< 저장 경로
        )
{
    return image.save(filePath, "JPEG", 100);
}

bool HdpConverter::saveToPng(
        const QString &filePath ///< 저장 경로
        )
{
    return image.save(filePath, "PNG");
}
