#include <QByteArray>
#include "hdpconverter.hpp"

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

ERR HdpConverter::PKCodecFactory_CreateDecoderFromMemory(
        const QByteArray &source
        )
{
    ERR err = WMP_errSuccess;

    const PKIID *pIID = nullptr;

    struct WMPStream *pStream = nullptr;

    // get decode PKIID
    GetImageDecodeIID(pExt, &pIID);

    // create stream
    Call(CreateWS_Memory(&pStream, static_cast<void*>(const_cast<char*>(source.data())), static_cast<size_t>(source.size())));

    // Create decoder
    Call(PKCodecFactory_CreateCodec(pIID, reinterpret_cast<void**>(&pDecoder)));

    // attach stream to decoder
    Call(pDecoder->Initialize(pDecoder, pStream));
    pDecoder->fStreamOwner = !0;

Cleanup:
    return err;
}

typedef struct tagBITMAPINFOHEADER{
    U32 uSize;
    I32 iWidth;
    I32 iHeight;
    I16 iPlanes;
    I16 iBitCount;
    U32 uCompression;
    U32 uImageSize;
    I32 iPelsPerMeterX;
    I32 iPelsPerMeterY;
    U32 uColorUsed;
    U32 uColorImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

ERR HdpConverter::WriteQImageHeader(
        PKImageEncode *pIE
        )
{
    ERR err = WMP_errSuccess;

    size_t cbLineS = 0;

    struct WMPStream* pS = pIE->pStream;
    BITMAPINFOHEADER bmpIH = {sizeof(bmpIH), 0, };

    pIE->cbPixel = 3;
    cbLineS = (pIE->cbPixel * pIE->uWidth + 3) / 4 * 4;

    bmpIH.iWidth = pIE->uWidth;
    bmpIH.iHeight = pIE->uHeight;
    bmpIH.iPlanes = 1;
    bmpIH.iBitCount = (I16)(8 * pIE->cbPixel);
    bmpIH.uImageSize = (U32)(cbLineS * pIE->uHeight);
    bmpIH.iPelsPerMeterX = (I32)(pIE->fResX * 39.37);
    bmpIH.iPelsPerMeterY = (I32)(pIE->fResY * 39.37);

    Call(pS->Write(pS, &bmpIH, sizeof(bmpIH)));

//    pIE->offPixel = pIE->offStart + bmpFH.uOffBits;
    pIE->fHeaderDone = !FALSE;

Cleanup:
    return err;
}

HdpConverter converter;

ERR PKImageEncode_WritePixels_QImage(
        PKImageEncode* pIE,
        U32 cLine,
        U8* pbPixel,
        U32 cbStride
        )
{
    ERR err = WMP_errSuccess;

    size_t cbLineM = 0;

    // header
    if (!pIE->fHeaderDone)
    {
        // WriteBMPHeader() also inits this object
        converter.WriteQImageHeader(pIE);
    }

    // calculate line size in memory and in stream
    cbLineM = pIE->cbPixel * pIE->uWidth;

    FailIf(cbStride < cbLineM, WMP_errInvalidParameter);

    converter.image = QImage(pbPixel, pIE->uWidth, pIE->uHeight, cbStride, QImage::Format_RGB888);
    pIE->idxCurrentLine += cLine;

Cleanup:
    return err;
}

HdpConverter::HdpConverter()
{
    pExt = const_cast<char*>(".bmp");
    pDecoder = nullptr;
}

/** 데이터를 읽어옴.
  */
void HdpConverter::loadHdpData(
        const QByteArray &source
        )
{
    PKFactory *pFactory = nullptr;
    PKCodecFactory *pCodecFactory = nullptr;
    CWMIStrCodecParam wmiSCP;

    PKPixelFormatGUID guidPixFormat = GUID_PKPixelFormat24bppRGB;

    PKCreateFactory(&pFactory, PK_SDK_VERSION);
    PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION);
    PKCodecFactory_CreateDecoderFromMemory(source);

    // < -- set color format -- >
    {
        PKPixelInfo PI; // 화소 정보
        PI.pGUIDPixFmt = &guidPixFormat;
        PixelFormatLookup(&PI, LOOKUP_FORWARD);

        pDecoder->WMP.wmiSCP.bfBitstreamFormat = SPATIAL; // spatial order
        pDecoder->WMP.wmiSCP.uAlphaMode = 0; // no alpha
        pDecoder->WMP.wmiSCP.sbSubband = SB_ALL; // All subbands included
        pDecoder->WMP.bIgnoreOverlap = FALSE;

        pDecoder->WMP.wmiI.cfColorFormat = PI.cfColorFormat; // 색상 형식
        pDecoder->WMP.wmiI.bdBitDepth = PI.bdBitDepth; // 색 깊이
        pDecoder->WMP.wmiI.cBitsPerUnit = PI.cbitUnit; // 비트 단위 유닛
    }

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

    // < -- 프레임 디코딩 -- >
    {
        // jxrlib 1.1에 프레임 수 세기, 프레임 선택 등과 같은 기능은 미구현된 상태임.
        //pDecoder->GetFrameCount(pDecoder, &cFrame); // 두 가지 스타일(다중 페이지 및 다중 해상도)로 표시되는 다중 프레임 이미지에 대한 정보를 반환합니다.
        //Call(pDecoder->SelectFrame(pDecoder, i + 1));
        struct WMPStream *pEncodeStream = nullptr; // 인코드 스트림
        PKImageEncode *pEncoder = nullptr; // 인코더
        PKFormatConverter *pConverter = nullptr; // 컨버터

        Float rX = 0.0, rY = 0.0; // 해상도
        PKRect rect = {0, 0, 0, 0}; // 크기

        // < -- 컨버터 초기화 -- >
        pCodecFactory->CreateFormatConverter(&pConverter);
        pConverter->Initialize(pConverter, pDecoder, pExt, guidPixFormat);

        // < -- 인코더 준비 -- >
        pFactory->CreateStreamFromMemory(&pEncodeStream, nullptr, 0); //의미 없음. 함수 포인터에 저절한 더미 함수를 배치 하기 위한 작업. 실질적 스트리밍 작업은 QImage, Qbuffer가 처리한다.
        pEncoder->WritePixels = PKImageEncode_WritePixels_QImage;
        if(pEncoder->bWMP) {
            pEncoder->Initialize(pEncoder, pEncodeStream, &wmiSCP, sizeof(wmiSCP));
        } else {
            pEncoder->Initialize(pEncoder, pEncodeStream, NULL, 0);
        }

        // < -- 출력 화소 포멧 설정 -- >
        pEncoder->SetPixelFormat(pEncoder, guidPixFormat);
        pEncoder->WMP.wmiSCP.bBlackWhite = pDecoder->WMP.wmiSCP.bBlackWhite;

        // < -- 출력 이미지 크기 설정 -- >
        rect.Width = static_cast<I32>(pDecoder->WMP.wmiI.cROIWidth);
        rect.Height = static_cast<I32>(pDecoder->WMP.wmiI.cROIHeight);
        pEncoder->SetSize(pEncoder, rect.Width, rect.Height);

        // < -- 출력 해상도 설정 -- >
        pDecoder->GetResolution(pDecoder, &rX, &rY); // 디코더의 해상도를 가져옵니다.
        pEncoder->SetResolution(pEncoder, rX, rY); // 인코더의 해상도를 디코더의 해상도로 설정합니다.

        // < -- 변환된 내용을 기록 -- >
        pEncoder->WriteSource = PKImageEncode_Transcode;
        pEncoder->WriteSource(pEncoder, pConverter, &rect);

        // < -- 인코더 할당 해제 -- >
        pEncoder->Release(&pEncoder);
    }

    // < -- 디코더 할당 해제 -- >
    pDecoder->Release(&pDecoder);
}

void HdpConverter::toJpeg(
        const QString &filePath ///< 저장 경로
        )
{
    image.save(filePath, "JPEG", 0);
}

void HdpConverter::toPng(
        const QString &filePath ///< 저장 경로
        )
{
    image.save(filePath, "PNG");
}
