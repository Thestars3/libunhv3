#include "bondchunkattr.hpp"
#include "fileinfo.hpp"

/** FileInfo 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in,
        FileInfo &fileInfo
        )
{
    BondChunkAttr NAME, POS4, CRC3, COMP;

    in >> fileInfo.FINF_ >> NAME >> POS4 >> CRC3 >> COMP;

    fileInfo.NAME_ = NAME.fromString();
    fileInfo.POS4_ = POS4.fromDword();
    fileInfo.CRC3_ = CRC3.fromDword();
    fileInfo.COMP_ = COMP.fromDword();

    return in;
}

BondChunkHeader FileInfo::FINF()
{
    return FINF_;
}

/** 파일명을 얻습니다.
  @return 파일명
  */
QString FileInfo::NAME()
{
    return NAME_;
}

/** hv3파일에서의 파일 청크 위치를 얻습니다.
  @return hv3파일에서의 파일 청크 위치
  */
uint FileInfo::POS4() const
{
    return POS4_;
}

/** 파일 데이터에 대한 CRC32를 얻습니다.
  @return 파일 데이터에 대한 CRC32
  */
uint FileInfo::CRC3()
{
    return CRC3_;
}

/** 파일에 대한 압축 방법을 얻습니다(0: 저장, 1~: 예약됨).
  @return 파일에 대한 압축 방법
  */
uint FileInfo::COMP()
{
    return COMP_;
}
