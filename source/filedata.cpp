#include "filedata.hpp"

/** FileData 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in,
        FileData &fileData
        )
{
    char *data;
    uint len = fileData.FILE_.chunkDataSize();
    in >> fileData.FILE_;
    in.readBytes(data, len);
    fileData.raw_data_.setRawData(data, len);
    delete data;

    return in;
}

/** 파일 데이터 청크를 얻습니다.
  @return 파일 데이터 청크
  */
BondChunkHeader FileData::FILE()
{
    return FILE_;
}

/** 파일 데이터를 얻습니다.
  @return 파일 데이터
  */
QByteArray FileData::raw_data()
{
    return raw_data_;
}
