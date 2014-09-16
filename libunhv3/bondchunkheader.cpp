#include "bondchunkheader.hpp"

const uint BondChunkHeader::CHUNK_SIZE = 16;

/** BondChunkHeader 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in, ///< 데이터 스트림
        BondChunkHeader &bondChunkHeader ///< BOND 포멧 청크 헤더 객체
        )
{
    char chunkName[5] = {'\0',};
    in.readRawData(chunkName, 4);
    bondChunkHeader.chunkName_ = chunkName;

    in >> bondChunkHeader.attrSize_;
    in >> bondChunkHeader.subChunkSize_;
    in >> bondChunkHeader.chunkDataSize_;

    return in;
}

/** 청크의 이름을 반환합니다.
  @return 청크의 이름
  */
QString BondChunkHeader::chunkName() const
{
    return chunkName_;
}

/** 속성 청크의 크기를 반환합니다.
  @return 속성 청크의 크기
  */
quint32 BondChunkHeader::attrSize() const
{
    return attrSize_;
}

/** 모든 하위 청크의 크기의 합을 반환합니다.
  @return 모든 하위 청크의 크기의 합
  */
quint32 BondChunkHeader::subChunkSize() const
{
    return subChunkSize_;
}

/** 현재 청크의 데이타 크기를 반환합니다.
  @return 현재 청크의 데이타 크기
  */
quint32 BondChunkHeader::chunkDataSize() const
{
    return chunkDataSize_;
}
