#include <QBuffer>
#include <QFileInfo>
#include <QDataStream>
#include <QImageReader>
#include "bondchunkattr.hpp"
#include "unhv3event.hpp"
#include "ufp.hpp"
#include "unhv3.hpp"

/** 압축 해제중 발생하는 여러가지 이벤트를 받아서 처리하고 싶을 경우, 이벤트를 콜백으로 받을 객체를 지정합니다.
  @return 성공여부.
  */
bool Unhv3::setEvent(
        Unhv3Event *event ///< 이벤트를 받을 객체의 포인터
        )
{
    if ( event == nullptr ) {
        return false;
    }

    event_ = event;

    return true;
}

/** 생성자.
  */
Unhv3::Unhv3() :
    openStatus(false),
    status(Unhv3Status::NO_ERROR),
    VERS_(0),
    FSIZ_(0),
    DIRE_(0),
    ENCR_(0),
    COPY_(QString::null),
    LINK_(QString::null),
    TITL_(QString::null),
    ISBN_(QString::null),
    WRTR_(QString::null),
    PUBL_(QString::null),
    DATE_(QString::null),
    COMT_(QString::null),
    MAKR_(QString::null),
    GENR_(QString::null)
{
}

/** 마지막으로 발생한 오류의 상세 내용을 확인한다.
  @return 상태코드.
  */
Unhv3Status Unhv3::lastError() const
{
    return status;
}

/** 메모리로 직접 해제해 보면서 파일의 손상 여부를 확인합니다.
  @return 파일의 손상여부
  */
bool Unhv3::testArchive()
{
    status = Unhv3Status::NOT_YET_IMPELEMENTED;
    return false;
}

/** open 메소드로 파일을 분석 할때, 체크썸이 올바른지 확인합니다.
  @return 파일 손상 여부
  */
bool Unhv3::isBrokenArchive()
{
    status = Unhv3Status::NOT_YET_IMPELEMENTED;
    return false;
}

/** 현재 열려있는 파일의 경로명을 리턴합니다.
  @return 파일의 경로명. 열려있지 않으면 QString::null을 리턴.
  */
QString Unhv3::filePathName() const
{
    if ( isOpened() ) {
        return file.fileName();
    }
    else {
        return QString::null;
    }
}

/** 현재 파일이 열려있는 상태인지 여부를 확인합니다.
  @return true : 열림; false : 안열림.
  */
bool Unhv3::isOpened() const
{
    return openStatus;
}

/** 이 객체의 내용을 초기화합니다.
  */
void Unhv3::clear()
{
    file.close();
    openStatus = false;
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

    int max = fileItemCount();
    QDir pwd = QDir::currentPath();
    pwd.cd(savePath);
    for(int i = 0; i < max; i++) {
        const FileInfo *fileInfo = getFileItem(i);
        QString fileName = fileInfo->NAME();
        extractOneAs(i, fileName);
    }
    event_->setComplete();
    pwd.cd(".");

    return true;
}

/** 현재 열려있는 파일의 파일 크기를 리턴합니다.
  @return 압축파일의 파일 크기.
  */
uint Unhv3::archiveFileSize() const
{
    return FSIZ_;
}

/** 현재 열려있는 파일의 HV3 포맷의 버전 정보를 리턴합니다.
  @return HV3 포맷의 버전 정보
  */
uint Unhv3::formatVersion() const
{
    return VERS_;
}

/** 현재 열려있는 파일의 GUID를 리턴합니다.
  @return 파일의 GUID
  */
QUuid Unhv3::archiveGuid() const
{
    return GUID_;
}

/** 현재 열려있는 파일의 UUID를 리턴합니다.
  @return 파일의 UUID
  */
QUuid Unhv3::archiveUuid() const
{
    return UUID_;
}

/** 현재 열려있는 파일의 만들어진 시간을 리턴합니다.
  @return 파일이 만들어진 시간
  */
QDateTime Unhv3::createdTime() const
{
    return FTIM_;
}

/** 현재 열려있는 파일의 책의 제본방식을 리턴합니다.
  @return 책의 제본방식 0: 정보없음, 1:left to right 2:right to left
  */
uint Unhv3::direction() const
{
    return DIRE_;
}

/** 현재 열려있는 파일의 암호화 방식을 리턴합니다.
  @return 파일의 암호화 방식, 0:암호화 없음
  */
uint Unhv3::encryptMethod() const
{
    return ENCR_;
}

/** 현재 열려있는 파일의 저작권 정보를 리턴합니다.
  @return 저작권 정보
  */
QString Unhv3::copyrightInformation() const
{
    return COPY_;
}

/** 현재 열려있는 파일의 관련 링크 URL을 리턴합니다.
  @return 관련 링크 URL
  */
QString Unhv3::relatedLink() const
{
    return LINK_;
}

/** 현재 열려있는 파일의 제목을 리턴합니다.
  @return 파일의 제목
  */
QString Unhv3::fileTitle() const
{
    return TITL_;
}

/** 현재 열려있는 파일의 책의 국제 표준 도서 번호 정보를 리턴합니다.
  @return 책의 ISBN 정보
  */
QString Unhv3::isbn() const
{
    return ISBN_;
}

/** 현재 열려있는 파일의 원 저작자를 리턴합니다.
  @return Original Writer
  */
QString Unhv3::originalWriter() const
{
    return WRTR_;
}

/** 현재 열려있는 파일의 출판인을 리턴합니다.
  @return Publisher
  */
QString Unhv3::publisher() const
{
    return PUBL_;
}

/** 현재 열려있는 파일의 원 출판 날자를 리턴합니다.
  @return Original Publishing date
  */
QString Unhv3::originalPublishingDate() const
{
    return DATE_;
}

/** 현재 열려있는 파일의 설명을 리턴합니다.
  @return Comment
  */
QString Unhv3::comment() const
{
    return COMT_;
}

/** 현재 열려있는 파일의 제작자를 리턴합니다.
  @return HV3 File maker
  */
QString Unhv3::fileMaker() const
{
    return MAKR_;
}

/** 현재 열려있는 파일의 장르를 리턴합니다.
  @return Genere
  */
QString Unhv3::genere() const
{
    return GENR_;
}

/** 현재 열려있는 파일 내의 파일 아이템 갯수를 리턴합니다.
  @return 압축파일 내의 파일 아이템 갯수
  */
int Unhv3::fileItemCount() const
{
    return LIST_.getFileItemCount();
}

/** 특정 파일 아이템의 정보를 가져옵니다.
  @return 지정된 인덱스의 파일 아이템 정보
  */
const FileInfo* Unhv3::getFileItem(
        int index ///< 파일 아이템의 인덱스
        ) const
{
    return LIST_.getFileItem(index);
}

/** 압축파일내의 한개의 파일만을 풀때 사용합니다.
  @return true : 작업 성공; false : 작업 중 오류 있음.
  */
bool Unhv3::extractOneTo(
        int index,
        const QString &savePath ///< 저장될 경로
        )
{
    QString filePathName = savePath + "/" + getFileItem(index)->NAME();
    return extractOneAs(index, filePathName);
}

/** 압축파일내의 한개의 파일만을 풀때 사용합니다. \n
  Unhv3::extractOneTo와 달리 파일명을 지정할 수 있습니다.
  @return true : 작업 성공; false : 작업 중 오류 있음.
  */
bool Unhv3::extractOneAs(
        int index,
        const QString &filePathName ///< 저장될 파일의 이름을 포함한 경로
        )
{
    const FileInfo *fileItem = LIST_.getFileItem(index);
    uint pos = fileItem->POS4();
    QByteArray raw_data;

    event_->setProgress(0);
    raw_data = BODY_.getFileData(pos)->raw_data();

    event_->setProgress(33);
    if ( ufp::computeCrc32(raw_data) != fileItem->CRC3() ) {
        status = Unhv3Status::CRC_ERROR;
        event_->setError(filePathName, status);
        return false;
    }
    if ( QString::compare(QFileInfo(fileItem->NAME()).suffix(), "hdp", Qt::CaseInsensitive) == 0 ) {
        QBuffer buffer(&raw_data);
        QImage image = QImageReader(&buffer, "HDP").read();
        event_->setProgress(66);

        bool b = true;
        if ( image.hasAlphaChannel() ) {
            b = image.save(filePathName, "PNG");
        }
        else {
            b = image.save(filePathName, "JPEG", 100);
        }
        event_->setProgress(99);

        if ( ! b ) {
            status = Unhv3Status::SAVE_FILE_ERROR;
            event_->setError(filePathName, status);
            return false;
        }
    }
    else {
        QFile file(filePathName);

        if ( ! file.open(QFile::WriteOnly) ) {
            status = Unhv3Status::SAVE_FILE_ERROR;
            event_->setError(filePathName, status);
            return false;
        }

        if ( file.write(raw_data) == -1 ) {
            status = Unhv3Status::SAVE_FILE_ERROR;
            event_->setError(filePathName, status);
            file.close();
            return false;
        }
        event_->setProgress(99);

        file.close();
    }

    event_->setProgress(100);
    return true;
}

/** filepath에 존재하는 hv3 파일을 엽니다.
  @return 성공 여부.
  */
bool Unhv3::open(
        const QString &filepath ///< 파일 경로
        )
{
    file.close();
    file.setFileName(filepath);
    openStatus = true;
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

    BondChunkAttr VERS, FSIZ, GUID, UUID, FTIM, DIRE, COPY, ENCR, LINK, TITL, ISBN, WRTR, PUBL, DATE, COMT, MAKR, GENR;

    fileStream_ >> VERS >> FSIZ >> HEAD_ >> GUID >> UUID >> FTIM >> DIRE >> ENCR >> COPY >> LINK >> TITL >> ISBN >> WRTR >> PUBL >> DATE >> COPY >> COMT >> MAKR >> GENR >> LIST_ >> BODY_;

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
