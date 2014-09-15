#include <QImage>
#include <JXRTest.h>
#include "hdpimageiohandler.hpp"

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

bool HdpImageIOHandler::canRead() const
{
    if ( ! device() ) {
        qWarning("HdpImageIOHandler::canRead() 불러올 장치를 지정하지 않았습니다.");
        return false;
    }

    // HDP의 magic numbe는 ASCII로 `II.'이고, 위치한 offset은 0이다.
    const qint64 oldPos = device()->pos();

    char head[3];
    qint64 readBytes = device()->read(head, sizeof(head));
    const bool readOk = ( readBytes == sizeof(head) );

    if ( device()->isSequential() ) {
        while (readBytes > 0) {
            device()->ungetChar(head[ readBytes-- - 1 ]);
        }
    }
    else {
        device()->seek(oldPos);
    }

    if ( ! readOk ) {
        return false;
    }

    return( head[0] == 'I' && head[1] == 'I' && head[2] == '.' );
}

bool HdpImageIOHandler::read(
        QImage *outImage
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
    Call(PKCodecFactory_CreateDecoderFromMemory(&pDecoder));

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
    //pEncoder->WritePixels = PKImageEncode_WritePixels_QImage;
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
    //pEncoder->WriteSource = PKImageEncode_Transcode;
    //Call(pEncoder->WriteSource(pEncoder, pConverter, &rect));
    PKImageEncode_Transcode(pEncoder, pConverter, &rect, outImage);

    // < -- 인코더 할당 해제 -- >
    pEncoder->Release(&pEncoder);

    // < -- 디코더 할당 해제 -- >
    pDecoder->Release(&pDecoder);

Cleanup:
    if ( Failed(err) ) {
        return false;
    }

    return true;
}

ERR HdpImageIOHandler::PKCodecFactory_CreateDecoderFromMemory(
        PKImageDecode **ppDecoder
        )
{
    ERR err = WMP_errSuccess;

    const PKIID *pIID = nullptr;

    struct WMPStream *pStream = nullptr;
    PKImageDecode *pDecoder = nullptr;

    QByteArray source;

    // get decode PKIID
    Call(GetImageDecodeIID(".hdp", &pIID));

    // create stream
    source = device()->readAll();
    Call(CreateWS_Memory(&pStream, static_cast<void*>(source.data()), static_cast<size_t>(source.size())));

    // Create decoder
    Call(PKCodecFactory_CreateCodec(pIID, reinterpret_cast<void**>(ppDecoder)));
    pDecoder = *ppDecoder;

    // attach stream to decoder
    Call(pDecoder->Initialize(pDecoder, pStream)); // 이 부분에서 이미지 데이터의 내용을 분석한다.
    pDecoder->fStreamOwner = !0;

Cleanup:
    return err;
}

ERR HdpImageIOHandler::WriteQImageHeader(
        PKImageEncode *pIE,
        QImage *image
        )
{
    //JXR 라이브러리에서 사용하는 해상도 단위는 인치. QT에서 쓰는건 미터단위.
    image->setDotsPerMeterX(inchConvertToMeter(pIE->fResX));
    image->setDotsPerMeterY(inchConvertToMeter(pIE->fResY));

    //헤더 기록 완료
    pIE->fHeaderDone = !FALSE;

    return WMP_errSuccess;
}

ERR HdpImageIOHandler::PKImageEncode_WritePixels_QImage(
        PKImageEncode *pIE,
        U32 cLine,
        U8 *pbPixel,
        U32 cbStride,
        QImage *outImage
        )
{
    ERR err = WMP_errSuccess;

    QImage::Format format;
    size_t cbLineM = 0;
    int size = pIE->uWidth * pIE->uHeight * 4;

    // < -- header -- >
    if ( ! pIE->fHeaderDone ) {
        WriteQImageHeader(pIE, outImage);
    }

    // calculate line size in memory and in stream
    cbLineM = pIE->cbPixel * pIE->uWidth;

    FailIf(cbStride < cbLineM, WMP_errInvalidParameter);

    if ( pIE->WMP.bHasAlpha ) {
        format = QImage::Format_ARGB32_Premultiplied;
        convertRgbaToArgb(pbPixel, size);
    }
    else {
        format = QImage::Format_RGB888;
    }

    *outImage = QImage(pbPixel, pIE->uWidth, pIE->uHeight, cbStride, format);
    pIE->idxCurrentLine += cLine;

Cleanup:
    return err;
}

/** RGBA 순서로된 바이트 배열을 ARGB 순서로 재배열한다.
  */
void HdpImageIOHandler::convertRgbaToArgb(
        uchar *data, ///< 재배열 할 데이터
        uint size        ///< 바이트 수
        )
{
    quint8 r, g, b, a;
    uchar *p = data;
    uint i = 0;
    while( i < size ) {
        r = p[0];
        g = p[1];
        b = p[2];
        a = p[3];

        *p++ = a;
        *p++ = r;
        *p++ = g;
        *p++ = b;

        i += 4;
    }
}

ERR HdpImageIOHandler::PKImageEncode_Transcode(
        PKImageEncode *pIE,
        PKFormatConverter *pFC,
        PKRect *pRect,
        QImage *outImage
        )
{
    ERR err = WMP_errSuccess;

    PKPixelFormatGUID enPFFrom = GUID_PKPixelFormatDontCare;
    PKPixelFormatGUID enPFTo = GUID_PKPixelFormatDontCare;

    PKPixelInfo pPIFrom;
    PKPixelInfo pPITo;

    U32 cbStrideTo = 0;
    U32 cbStrideFrom = 0;
    U32 cbStride = 0;

    U8* pb = nullptr;

    CWMTranscodingParam cParam = {0};

    // get pixel format
    Call(pFC->GetSourcePixelFormat(pFC, &enPFFrom));
    Call(pFC->GetPixelFormat(pFC, &enPFTo));

    // calc common stride
    pPIFrom.pGUIDPixFmt = &enPFFrom;
    PixelFormatLookup(&pPIFrom, LOOKUP_FORWARD);

    pPITo.pGUIDPixFmt = &enPFTo;
    PixelFormatLookup(&pPITo, LOOKUP_FORWARD);

    cbStrideFrom = (BD_1 == pPIFrom.bdBitDepth ? ((pPIFrom.cbitUnit * pRect->Width + 7) >> 3) : (((pPIFrom.cbitUnit + 7) >> 3) * pRect->Width));
    if (&GUID_PKPixelFormat12bppYUV420 == pPIFrom.pGUIDPixFmt || &GUID_PKPixelFormat16bppYUV422 == pPIFrom.pGUIDPixFmt) {
        cbStrideFrom >>= 1;
    }

    cbStrideTo = (BD_1 == pPITo.bdBitDepth ? ((pPITo.cbitUnit * pIE->uWidth + 7) >> 3) : (((pPITo.cbitUnit + 7) >> 3) * pIE->uWidth));
    if (&GUID_PKPixelFormat12bppYUV420 == pPITo.pGUIDPixFmt || &GUID_PKPixelFormat16bppYUV422 == pPITo.pGUIDPixFmt) {
        cbStrideTo >>= 1;
    }

    cbStride = qMax(cbStrideFrom, cbStrideTo);

    if(pIE->bWMP){
        cParam.cLeftX = pFC->pDecoder->WMP.wmiI.cROILeftX;
        cParam.cTopY = pFC->pDecoder->WMP.wmiI.cROITopY;
        cParam.cWidth = pFC->pDecoder->WMP.wmiI.cROIWidth;
        cParam.cHeight = pFC->pDecoder->WMP.wmiI.cROIHeight;
        cParam.oOrientation = pFC->pDecoder->WMP.wmiI.oOrientation;
        cParam.uAlphaMode = pFC->pDecoder->WMP.wmiSCP.uAlphaMode;
        cParam.bfBitstreamFormat = pFC->pDecoder->WMP.wmiSCP.bfBitstreamFormat;
        cParam.sbSubband = pFC->pDecoder->WMP.wmiSCP.sbSubband;
        cParam.bIgnoreOverlap = pFC->pDecoder->WMP.bIgnoreOverlap;

        Call(pIE->Transcode(pIE, pFC->pDecoder, &cParam));
    }
    else {
        // actual dec/enc with local buffer
        Call(PKAllocAligned((void **) &pb, cbStride * pRect->Height, 128));
        Call(pFC->Copy(pFC, pRect, pb, cbStride));
        //Call(pIE->WritePixels(pIE, pRect->Height, pb, cbStride));
        Call(PKImageEncode_WritePixels_QImage(pIE, pRect->Height, pb, cbStride, outImage));
    }

Cleanup:
    PKFreeAligned(reinterpret_cast<void**>(&pb));
    return err;
}
