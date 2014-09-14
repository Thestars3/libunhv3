#include <QTextCodec>
#include "bondchunkattr.hpp"

/** hv3 포멧의 GUID 타입을 QUuid 타입으로 저장합니다.\n
  마이크로스트社에서 UUID 표준을 구현한 것이 GUID이며, 데이터 구조상으로 UUID와 동일하다.
  @return QUuid
  */
QUuid BondChunkAttr::fromGuid()
{
    QDataStream dataStream;
    uint l;
    ushort w1, w2;
    uchar b[8];
    quint32 u32;
    quint16 u16;
    quint8 u8;

    dataStream.readBytes(reinterpret_cast<char*&>(attrData_), attrDataSize_);
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
QUuid BondChunkAttr::fromUuid()
{
    QDataStream dataStream;
    dataStream.readBytes(reinterpret_cast<char*&>(attrData_), attrDataSize_);
    QUuid uuid;
    dataStream >> uuid;
    return uuid;
}

/** hv3 포멧의 DWORD 타입을 uint 타입으로 저장합니다.
  @return uint
  */
uint BondChunkAttr::fromDword()
{
    QDataStream dataStream;
    quint32 i;
    dataStream.readBytes(reinterpret_cast<char*&>(attrData_), attrDataSize_);
    dataStream >> i;
    return i;
}

/** hv3 포멧의 STRING 타입을 QString 타입으로 바꾸어 저장합니다.
  @return QString
  */
QString BondChunkAttr::fromString()
{
    return textCodec->toUnicode(reinterpret_cast<char*>(attrData_), attrDataSize_);
}

/** hv3 포멧의 FILETIME 타입을 QDateTime 타입으로 바꾸어 저장합니다.
  @return QDateTime
  */
QDateTime BondChunkAttr::fromFiletime()
{
    QDataStream dataStream;
    QDateTime dateTime;
    qint32 nYear, nMonth, nDay, nHour, nMin, nSec;
    dataStream.readBytes(reinterpret_cast<char*&>(attrData_), attrDataSize_);
    dataStream >> nYear >> nMonth >> nDay >> nHour >> nMin >> nSec;
    dateTime.setDate(QDate(nYear, nMonth, nDay));
    dateTime.setTime(QTime(nHour, nMin, nSec));
    return dateTime;
}

/** 생성자.
  */
BondChunkAttr::BondChunkAttr()
{
    textCodec = QTextCodec::codecForName("UCS-2 LE");
    attrData_ = nullptr;
}

/** 소멸자.
  */
BondChunkAttr::~BondChunkAttr()
{
    if ( attrData_ != nullptr ) {
        delete attrData_;
    }
}

/** BondChunkAttr 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in,
        BondChunkAttr &bondChunkAttr
        )
{
    char attrName[5] = {'\0',};
    in.readRawData(attrName, 4);
    bondChunkAttr.attrName_ = attrName;

    in >> bondChunkAttr.attrDataSize_;

    in.readBytes(reinterpret_cast<char*&>(bondChunkAttr.attrData_), bondChunkAttr.attrDataSize_);

    return in;
}

/** 속성의 이름을 반환한다.
  @return 속성의 이름
  */
QString BondChunkAttr::attrName()
{
    return attrName_;
}

/** 속성 정보의 크기를 반환한다.
  @return 속성 정보의 크기
  */
quint32 BondChunkAttr::attrDataSize()
{
    return attrDataSize_;
}

/** 속성 데이터를 반환한다.
  @return 속성 데이터
  */
quint8* BondChunkAttr::attrData()
{
    return attrData_;
}
