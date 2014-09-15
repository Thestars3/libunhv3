#include "unhv3status.hpp"

Unhv3Status& Unhv3Status::operator=(
        const Status &status ///< 상태
        )
{
    status_ = status;
    return *this;
}

Unhv3Status::Status Unhv3Status::getStatusCode()
{
    return status_;
}

QString Unhv3Status::getMessage()
{
    switch (status_) {
    case NO_ERROR:
        return QString::fromUtf8("오류가 없습니다.");

    case FILE_NOT_EXIST:
        return QString::fromUtf8("파일 존재하지 않습니다.");

    case CANT_READ_FILE:
        return QString::fromUtf8("파일을 읽을수 없습니다.");

    case NOT_HV3_FORMAT:
        return QString::fromUtf8("hv3 포멧이 아닙니다.");

    case CRC_ERROR:
        return QString::fromUtf8("CRC 오류가 존재합니다.");

    case SAVEPATH_NOT_EXIST:
        return QString::fromUtf8("지정된 저장 경로가 존재하지 않습니다.");

    case SAVEPATH_IS_NOT_DIR:
        return QString::fromUtf8("지정된 저장 경로는 폴더가 아닙니다.");

    case NOT_YET_IMPELEMENTED:
        return QString::fromUtf8("아직 구현되지 않은 기능을 사용하려 했습니다.");

    default:
        return QString::fromUtf8("알 수 없는 오류 코드입니다.");
    }
}

Unhv3Status::Unhv3Status(
        Status status
        ) :
    status_(status)
{
}
