#include "hdpconverter.hpp"

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

/** Encoder factory on file extension.
  */
void HdpConverter::WmpDecAppCreateEncoder(
        PKImageEncode **ppIE
        )
{
    const PKIID *pIID = nullptr;

    // get encod PKIID
    GetTestEncodeIID(pExt, &pIID);

    // Create encoder
    PKTestFactory_CreateCodec(pIID, reinterpret_cast<void**>(ppIE));
}

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

HdpConverter::HdpConverter()
{
    pExt = const_cast<char*>(".bmp");
    pDecoder = nullptr;
}

/** raw_data에 저장된 내용을 HDP파일로 읽어와서 BMP로 저장합니다.
  @todo jxr 인코딩 데이터를 스트림 형태로 저장하도록 한뒤, QImage 관련 클래스를 사용하여 jpg로 변환하게 한다. \n
 스트림 형태로 저장할때, 저장 공간과 크기가 사전에 요구되는 부분을 처리하기 위해, 변환 되기 전에 할당될 크기를 알고 그 공간을 미리 만들어 줘야 한다.
  @return HDP 파일 데이터
  */
QByteArray HdpConverter::convertToBmp(
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
    U32 cFrame = 0;
    pDecoder->GetFrameCount(pDecoder, &cFrame); // 두 가지 스타일(다중 페이지 및 다중 해상도)로 표시되는 다중 프레임 이미지에 대한 정보를 반환합니다.
    for (U32 i = 0; ; ++i) {
        struct WMPStream *pEncodeStream = nullptr;
        PKImageEncode *pEncoder = nullptr;

        PKFormatConverter *pConverter = nullptr;

        Float rX = 0.0, rY = 0.0; // 해상도
        PKRect rect = {0, 0, 0, 0}; // 크기

        // < -- 컨버터 초기화 -- >
        pCodecFactory->CreateFormatConverter(&pConverter);
        pConverter->Initialize(pConverter, pDecoder, pExt, guidPixFormat);

        // < -- 인코더 준비 -- >
        QByteArray dest;
        quint8 *data = static_cast<quint8*>(calloc(8, 1));
        quint32 size = 8;
        pFactory->CreateStreamFromMemory(&pEncodeStream, data, size);
        WmpDecAppCreateEncoder(&pEncoder); // BMP 인코더 준비
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
//        QImage::fromData(data, );
//        QImage image(data, rect.Width, rect.Height, QImage::Format_RGB888);

        // < -- 인코더 할당 해제 -- >
        pEncoder->Release(&pEncoder);

        // < -- 마지막 프레임 여부 검사 -- >
        if ( i + 1 == cFrame ) { // multi-frame support NYI
            break;
        }

        // < -- 원본에서 다음 프레임 선택 -- >
        pDecoder->SelectFrame(pDecoder, i + 1);
    }

    // < -- 디코더 할당 해제 -- >
    pDecoder->Release(&pDecoder);

    return QByteArray();
}
