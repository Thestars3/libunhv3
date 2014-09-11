#include "hdpconverter.hpp"

void HdpConverter::WmpDecAppInitDefaultArgs(WMPDECAPPARGS* args)
{
    memset(args, 0, sizeof(*args));
    args->guidPixFormat = GUID_PKPixelFormatDontCare;
//    args->bFlagRGB_BGR = FALSE; //default BGR
    args->bVerbose = FALSE;
    args->tThumbnailFactor = 0;
    args->oOrientation = O_NONE;
    args->cPostProcStrength = 0;
    args->uAlphaMode = 255;
    args->sbSubband = SB_ALL;
}

void HdpConverter::WmpDecAppParseArgs(WMPDECAPPARGS* args)
{
    WmpDecAppInitDefaultArgs(args);

    args->szInputFile= nullptr; // 입력 파일 경로
    args->szOutputFile = nullptr; // 저장 파일 경로
    args->guidPixFormat = GUID_PKPixelFormat24bppRGB;
}
//================================================================
// Encoder factory on file extension
//================================================================
void HdpConverter::WmpDecAppCreateEncoderFromExt(
    PKCodecFactory* pCFactory,
    const char* szExt,
    PKImageEncode** ppIE)
{
    const PKIID* pIID = NULL;

    UNREFERENCED_PARAMETER( pCFactory );

    // get encod PKIID
    GetTestEncodeIID(szExt, &pIID);

    // Create encoder
    PKTestFactory_CreateCodec(pIID, reinterpret_cast<void**>(ppIE));
}

/** raw_data에 저장된 내용을 HDP파일로 읽어와서 BMP로 저장합니다.
  @return HDP 파일 데이터
  */
QByteArray HdpConverter::convertToBmp()
{
    PKFactory* pFactory = NULL;
    PKCodecFactory* pCodecFactory = NULL;
    PKImageDecode* pDecoder = NULL;

    WMPDECAPPARGS args = {0};
    char* pExt = NULL;
    U32 cFrame = 0;
    PKPixelInfo PI;

    pExt = const_cast<char*>(".bmp");

    pCodecFactory->CreateDecoderFromFile(args.szInputFile, &pDecoder);

    //==== set default color format
    PI.pGUIDPixFmt = &args.guidPixFormat;

    PixelFormatLookup(&PI, LOOKUP_FORWARD);

    args.uAlphaMode = 0;

    pDecoder->WMP.wmiSCP.bfBitstreamFormat = args.bfBitstreamFormat;
    pDecoder->WMP.wmiSCP.uAlphaMode = args.uAlphaMode;
    pDecoder->WMP.wmiSCP.sbSubband = args.sbSubband;
    pDecoder->WMP.bIgnoreOverlap = args.bIgnoreOverlap;

    pDecoder->WMP.wmiI.cfColorFormat = PI.cfColorFormat;

    pDecoder->WMP.wmiI.bdBitDepth = PI.bdBitDepth;
    pDecoder->WMP.wmiI.cBitsPerUnit = PI.cbitUnit;

    //==== Validate thumbnail decode parameters =====
    pDecoder->WMP.wmiI.cThumbnailWidth = pDecoder->WMP.wmiI.cWidth;
    pDecoder->WMP.wmiI.cThumbnailHeight = pDecoder->WMP.wmiI.cHeight;
    pDecoder->WMP.wmiI.bSkipFlexbits = FALSE;

    args.rLeftX = args.rTopY = 0;
    args.rWidth = pDecoder->WMP.wmiI.cThumbnailWidth;
    args.rHeight = pDecoder->WMP.wmiI.cThumbnailHeight;

    pDecoder->WMP.wmiI.cROILeftX = args.rLeftX;
    pDecoder->WMP.wmiI.cROITopY = args.rTopY;
    pDecoder->WMP.wmiI.cROIWidth = args.rWidth;
    pDecoder->WMP.wmiI.cROIHeight = args.rHeight;

    pDecoder->WMP.wmiI.oOrientation = args.oOrientation;

    pDecoder->WMP.wmiI.cPostProcStrength = args.cPostProcStrength;

    pDecoder->WMP.wmiSCP.bVerbose = args.bVerbose;

    pDecoder->GetFrameCount(pDecoder, &cFrame);

    {
        struct WMPStream* pEncodeStream = NULL;
        PKImageEncode* pEncoder = NULL;

        PKFormatConverter* pConverter = NULL;

        Float rX = 0.0, rY = 0.0;
        PKRect rect = {0, 0, 0, 0};

        pCodecFactory->CreateFormatConverter(&pConverter);

        pConverter->Initialize(pConverter, pDecoder, pExt, args.guidPixFormat);

        pFactory->CreateStreamFromFilename(&pEncodeStream, args.szOutputFile, "wb");

        WmpDecAppCreateEncoderFromExt(pCodecFactory, pExt, &pEncoder);

        pEncoder->Initialize(pEncoder, pEncodeStream, NULL, 0);

        pEncoder->SetPixelFormat(pEncoder, args.guidPixFormat);
        pEncoder->WMP.wmiSCP.bBlackWhite = pDecoder->WMP.wmiSCP.bBlackWhite;

        rect.Width = (I32)(pDecoder->WMP.wmiI.cROIWidth);
        rect.Height = (I32)(pDecoder->WMP.wmiI.cROIHeight);

        pEncoder->SetSize(pEncoder, rect.Width, rect.Height);

        pDecoder->GetResolution(pDecoder, &rX, &rY);

        pEncoder->SetResolution(pEncoder, rX, rY);

        pEncoder->WriteSource = PKImageEncode_Transcode;
        pEncoder->WriteSource(pEncoder, pConverter, &rect);

        pEncoder->Release(&pEncoder);
    }

    pDecoder->Release(&pDecoder);

    return QByteArray();
}
