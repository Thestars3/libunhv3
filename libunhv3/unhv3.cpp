#include <QFileInfo>
#include <QDataStream>
#include "HdpConverter/hdpconverter.hpp"
#include "bondchunkattr.hpp"
#include "unhv3.hpp"

Unhv3::Unhv3()
{
    status = Unhv3Status::NO_ERROR;
    converter = new HdpConverter();
}

Unhv3::~Unhv3()
{
    delete converter;
}

/** 마지막으로 발생한 오류의 상세 내용을 확인한다.
  @return 상태코드.
  */
Unhv3Status Unhv3::getLastError()
{
    return status;
}

bool Unhv3::testArchive()
{
    status = Unhv3Status::NOT_YET_IMPELEMENTED;
    return false;
}

bool Unhv3::isBrokenArchive()
{
    status = Unhv3Status::NOT_YET_IMPELEMENTED;
    return false;
}

QString Unhv3::filePathName()
{
    return file.fileName();
}

bool Unhv3::isOpened()
{
    return file.isOpen();
}

void Unhv3::clear()
{
    file.close();
    delete converter;
    converter = new HdpConverter();
    HV30_ = BondChunkHeader();
    VERS_ = 0;
    FSIZ_ = 0;
    HEAD_ = BondChunkHeader();
    GUID_ = QUuid();
    UUID_ = QUuid();
    FTIM_ = QDateTime();
    DIRE_ = 0;
    ENCR_ = 0;
    COPY_ = QString::null;
    LINK_ = QString::null;
    TITL_ = QString::null;
    ISBN_ = QString::null;
    WRTR_ = QString::null;
    PUBL_ = QString::null;
    DATE_ = QString::null;
    COMT_ = QString::null;
    MAKR_ = QString::null;
    GENR_ = QString::null;
    LIST_ = FileInfoList();
    BODY_ = FileDataStorage();
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

    if ( chdir(savePath.toUtf8().constData()) == -1 ) {
        return false;
    }

    int max = getFileItemCount();
    QDir pwd = QDir::currentPath();
    pwd.cd(savePath);
    for(int i = 0; i < max; i++) {
        const FileInfo *fileInfo = getFileItem(i);
        QString fileName = fileInfo->NAME();
        extractOneAs(i, fileName);
    }
    pwd.cd(".");

    return true;
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
        ) const
{
    return LIST_.getFileItem(index);
}

bool Unhv3::extractOneTo(
        int index,
        const QString &savePath
        )
{
    QString filePathName = savePath + "/" + getFileItem(index)->NAME();
    return extractOneAs(index, filePathName);
}

bool Unhv3::extractOneAs(
        int index,
        const QString &filePathName
        )
{
    bool success = true;
    uint pos = LIST_.getFileItem(index)->POS4();
    QByteArray *raw_data = BODY_.getFileData(pos)->raw_data();

    try {
        converter->setData(*raw_data);
        if ( converter->hasAlphaChannel() ) {
            converter->saveToPng(filePathName);
        }
        else {
            converter->saveToJpeg(filePathName);
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
