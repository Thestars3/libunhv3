#include <QTextCodec>
#include "bondchunkattr.hpp"

/** 생성자.
  */
BondChunkAttr::BondChunkAttr(
        const QString &attrName
        )
{
    attrName_ = attrName;
}

/** hv3 포멧의 GUID 타입을 QUuid 타입으로 저장합니다.\n
  마이크로스트社에서 UUID 표준을 구현한 것이 GUID이며, 데이터 구조상으로 UUID와 동일하다.
  @return QUuid
  */
QUuid BondChunkAttr::fromGuid() const
{
    QDataStream dataStream(attrData_);
    uint l;
    ushort w1, w2;
    uchar b[8];
    quint32 u32;
    quint16 u16;
    quint8 u8;

    dataStream >> u32;
    l = u32;
    dataStream >> u16;
    w1 = u16;
    dataStream >> u16;
    w2 = u16;
    for (int i = 0; i < 8; i++) {
        dataStream >> u8;
        b[i] = u8;
    }

    return QUuid(l, w1, w2, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
}

/** hv3 포멧의 UUID 타입을 QUuid 타입으로 저장합니다.
  @return QUuid
  */
QUuid BondChunkAttr::fromUuid() const
{
    QDataStream dataStream(attrData_);
    QUuid uuid;
    dataStream >> uuid;
    return uuid;
}

/** hv3 포멧의 DWORD 타입을 uint 타입으로 저장합니다.
  @return uint
  */
uint BondChunkAttr::fromDword() const
{
    QDataStream dataStream(attrData_);
    quint32 i;
    dataStream >> i;
    return i;
}

/** hv3 포멧의 STRING 타입을 QString 타입으로 바꾸어 저장합니다.
  @return QString
  */
QString BondChunkAttr::fromString() const
{
    return textCodec->toUnicode(attrData_);
}

/** hv3 포멧의 FILETIME 타입을 QDateTime 타입으로 바꾸어 저장합니다.
  @return QDateTime
  */
QDateTime BondChunkAttr::fromFiletime() const
{
    QDataStream dataStream(attrData_);
    QDateTime dateTime;
    qint32 nYear, nMonth, nDay, nHour, nMin, nSec;
    dataStream >> nYear >> nMonth >> nDay >> nHour >> nMin >> nSec;
    dateTime.setDate(QDate(nYear, nMonth, nDay));
    dateTime.setTime(QTime(nHour, nMin, nSec));
    return dateTime;
}

const QTextCodec *BondChunkAttr::textCodec = QTextCodec::codecForName("UCS-2 LE");

/** BondChunkAttr 역직렬화 수행자.
  @throw 포멧 경계가 잘못될 경우 std::exception를 던집니다.
  */
QDataStream& operator>>(
        QDataStream &in, ///< 데이터 스트림
        BondChunkAttr &bondChunkAttr ///< BOND 포멧 속성 청크 객체
        )
{
    char attrName[5] = {'\0',};
    quint32 attrDataSize; // 속성 정보의 크기
    quint8 *attrData;     // 속성 데이터. attrDataSize_ 만큼의 크기를 가진다.

    in.readRawData(attrName, 4);
    if ( bondChunkAttr.attrName_ != attrName ) {
        throw std::exception();
    }

    in >> attrDataSize;

    in.readBytes(reinterpret_cast<char*&>(attrData), attrDataSize);
    bondChunkAttr.attrData_.setRawData(reinterpret_cast<const char*>(attrData), attrDataSize);
    delete attrData;

    return in;
}

/** 속성의 이름을 반환한다.
  @return 속성의 이름
  */
QString BondChunkAttr::attrName() const
{
    return attrName_;
}

/** 속성 데이터를 반환한다.
  @return 속성 데이터
  */
QByteArray BondChunkAttr::attrData() const
{
    return attrData_;
}
