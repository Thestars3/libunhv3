#include <windowsmediaphoto.h>
#include "wmp_err.hpp"

WMP_err::WMP_err(
        ERR err ///< libjxr 오류코드
        )
{
    err_ = err;
}

ERR WMP_err::getErrorCode()
{
    return err_;
}

QString WMP_err::getErrorMessage()
{
    switch(err_){
    case WMP_errSuccess:
        return QString::fromUtf8("작업 처리에 성공했습니다.");
        break;
    case WMP_errFail:
        return QString::fromUtf8("작업 처리에 실패했습니다.");
        break;
    case WMP_errNotYetImplemented:
        return QString::fromUtf8("아직 구현되지 않은 기능을 사용했습니다.");
        break;
    case WMP_errAbstractMethod:
        return QString::fromUtf8("추상 메소드를 호출했습니다.");
        break;
    case WMP_errOutOfMemory:
        return QString::fromUtf8("잘못된 메모리 주소에 엑세스 하려 합니다.");
        break;
    case WMP_errFileIO:
        return QString::fromUtf8("파일 포인터가 잘못되었습니다.");
        break;
    case WMP_errBufferOverflow:
        return QString::fromUtf8("버퍼 오버 플로우가 일어나려 했습니다.");
        break;
    case WMP_errInvalidParameter:
        return QString::fromUtf8("잘못된 매개변수 입니다.");
        break;
    case WMP_errInvalidArgument:
        return QString::fromUtf8("잘못된 인자입니다.");
        break;
    case WMP_errUnsupportedFormat:
        return QString::fromUtf8("지원하지 않는 포멧입니다.");
        break;
    case WMP_errIncorrectCodecVersion:
        return QString::fromUtf8("부정확한 코덱 버전입니다.");
        break;
    case WMP_errIndexNotFound:
        return QString::fromUtf8("인덱스를 찾는데 실패했습니다.");
        break;
    case WMP_errOutOfSequence:
        return QString::fromUtf8("the caller called us after we've already written the header out");
        break;
    case WMP_errNotInitialized:
        return QString::fromUtf8("초기화되지 않았습니다.");
        break;
    case WMP_errMustBeMultipleOf16LinesUntilLastCall:
        return QString::fromUtf8("Unless this is the last call, reject inputs which are not multiples of 16");
        break;
    case WMP_errPlanarAlphaBandedEncRequiresTempFile:
        return QString::fromUtf8("평면 알파 부호화에는 필요한 임시파일이 없습니다.");
        break;
    case WMP_errAlphaModeCannotBeTranscoded:
        return QString::fromUtf8("영상신호방식의 알파 모드를 변환 할 수 없습니다.");
        break;
    case WMP_errIncorrectCodecSubVersion:
        return QString::fromUtf8("부정확한 코덱 서브 버전입니다.");
        break;
    default:
        return QString::fromUtf8("알 수 없는 오류코드입니다.");
    }
}

void WMP_err::raise()
{
    throw *this;
}

const QtConcurrent::Exception* WMP_err::clone()
{
    return new WMP_err(err_);
}
