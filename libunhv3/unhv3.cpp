#include <QFileInfo>
#include <QDataStream>
#include "HdpConverter/hdpconverter.hpp"
#include "bondchunkattr.hpp"
#include "unhv3.hpp"

Unhv3::Unhv3()
{
    status = Unhv3Status::NO_ERROR;
}

/** 마지막으로 발생한 오류의 상세 내용을 확인한다.
  @return 상태코드.
  */
Unhv3Status Unhv3::getLastError()
{
    return status;
}

/** 지정된 경로에 파일을 모두 푼다.
  @return 성공여부.
  */
bool Unhv3::extractAllTo(
        const QString &savePath ///< 저장 경로
        )
{
    QFileInfo pathInfo(savePath);

    if ( ! pathInfo.exists() ) {
        status = Unhv3Status::SAVEPATH_NOT_EXIST;
        return false;
    }

    if ( ! pathInfo.isDir() ) {
        status = Unhv3Status::SAVEPATH_IS_NOT_DIR;
        return false;
    }

    return false;
}

/** 압축파일 내의 파일 아이템 갯수를 리턴합니다.
  @return 압축파일 내의 파일 아이템 갯수
  */
int Unhv3::getFileItemCount()
{
    return LIST_.getFileItemCount();
}

const FileInfo* Unhv3::getFileItem(
        int index
        )
{
    return LIST_.getFileItem(index);
}

bool Unhv3::extractOneTo(
        int index,
        const QString &savePath
        )
{
    bool success = true;
    uint pos = LIST_.getFileItem(index)->POS4();
    QByteArray *raw_data = BODY_.getFileData(pos)->raw_data();

    try {
        HdpConverter::getInstance()->setData(*raw_data);
        if ( HdpConverter::getInstance()->hasAlphaChannel() ) {
            HdpConverter::getInstance()->saveToPng(savePath);
        }
        else {
            HdpConverter::getInstance()->saveToJpeg(savePath);
        }
    }
    catch (WMP_err &err) {
        success = false;
    }

    delete raw_data;

    return success;
}

/** filepath에 존재하는 hv3 파일을 엽니다.
  @return 성공 여부.
  */
bool Unhv3::open(
        const QString &filepath ///< 파일 경로
        )
{
    if ( file.isOpen() ) {
        file.close();
    }
    file.setFileName(filepath);
    fileStream_.setDevice(&file);

    QFileInfo fileInfo(file);

    if ( ! fileInfo.exists() ) {
        status = Unhv3Status::FILE_NOT_EXIST;
        return false;
    }

    if ( ! fileInfo.isReadable() ) {
        status = Unhv3Status::CANT_READ_FILE;
        return false;
    }

    if ( fileInfo.size() < 4 ) {
        status = Unhv3Status::NOT_HV3_FORMAT;
        return false;
    }

    fileStream_ >> HV30_;

    if ( HV30_.chunkName() != "HV30" ) {
        status = Unhv3Status::NOT_HV3_FORMAT;
        return false;
    }

    BondChunkAttr VERS, FSIZ, GUID, UUID, FTIM, DIRE, COPY, ENCR, LINK,
            TITL, ISBN, WRTR, PUBL, DATE, COMT, MAKR, GENR;

    fileStream_ >> VERS >> FSIZ >> HEAD_ >> GUID >> UUID >> FTIM >> DIRE
               >> ENCR >> COPY >> LINK >> TITL >> ISBN >> WRTR >> PUBL
               >> DATE >> COPY >> COMT >> MAKR >> GENR >> LIST_ >> BODY_;

    VERS_ = VERS.fromDword();
    FSIZ_ = FSIZ.fromDword();
    GUID_ = GUID.fromGuid();
    UUID_ = UUID.fromUuid();
    FTIM_ = FTIM.fromFiletime();
    DIRE_ = DIRE.fromDword();
    ENCR_ = ENCR.fromDword();
    COPY_ = COPY.fromString();
    LINK_ = LINK.fromString();
    TITL_ = TITL.fromString();
    ISBN_ = ISBN.fromString();
    WRTR_ = WRTR.fromString();
    PUBL_ = PUBL.fromString();
    DATE_ = DATE.fromString();
    COMT_ = COMT.fromString();
    MAKR_ = MAKR.fromString();
    GENR_ = GENR.fromString();

    status = Unhv3Status::NO_ERROR;
    return true;
}
