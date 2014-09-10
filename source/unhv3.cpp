#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include "bondchunkattr.hpp"
#include "unhv3.hpp"

/** filepath에 존재하는 hv3 파일을 엽니다.
  @return 상태 코드. 성공적으로 끝났다면, Unhv3Status::NO_ERROR를 반환.
  */
Unhv3Status Unhv3::open(
        const QString &filepath ///< 파일 경로
        )
{
    QFile file(filepath);
    QDataStream dataStream(&file);
    QFileInfo fileInfo(file);

    if ( ! fileInfo.exists() ) {
        return Unhv3Status::FILE_NOT_EXIST;
    }

    if ( ! fileInfo.isReadable() ) {
        return Unhv3Status::CANT_READ_FILE;
    }

    if ( ! fileInfo.isFile() ) {
        return Unhv3Status::IS_NOT_FILE;
    }

    if ( fileInfo.size() < 4 ) {
        return Unhv3Status::NOT_HV3_FORMAT;
    }

    dataStream >> HV30_;

    if ( HV30_.chunkName() != "HV30" ) {
        return Unhv3Status::NOT_HV3_FORMAT;
    }

    BondChunkAttr VERS, FSIZ, GUID, UUID, FTIM, DIRE, COPY, ENCR, LINK,
            TITL, ISBN, WRTR, PUBL, DATE, COMT, MAKR, GENR;

    dataStream >> VERS >> FSIZ >> HEAD_ >> GUID >> UUID >> FTIM >> DIRE
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

    return Unhv3Status::NO_ERROR;
}
