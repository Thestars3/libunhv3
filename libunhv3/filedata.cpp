#include <QFile>
#include "filedata.hpp"

/** FileData 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in, ///< 데이터 스트림
        FileData &fileData ///< 파일 데이터 객체
        )
{
    uint len;
    fileData.pos_ = in.device()->pos();
    in >> fileData.FILE_;
    len = fileData.FILE_.chunkDataSize();

    // raw_data
    fileData.raw_data_pos = in.device()->pos();
    fileData.raw_data_len = len;
    in.skipRawData(len);

    fileData.fileStream_ = &in;

    return in;
}

/** 파일 데이터 청크를 얻습니다.
  @return 파일 데이터 청크
  */
BondChunkHeader FileData::FILE() const
{
    return FILE_;
}

/** 파일 데이터를 얻습니다.
  @return 파일 데이터.
  */
QByteArray FileData::raw_data() const
{
    char *data = nullptr;
    QByteArray raw_data;

    fileStream_->device()->seek(raw_data_pos);
    fileStream_->readBytes(data, const_cast<uint&>(raw_data_len));
    raw_data.setRawData(data, raw_data_len);
    delete data;

    return raw_data;
}

/** 압축 파일 내에서의 파일 데이터 위치를 반환합니다.
  @return 압축 파일 내에서의 파일 데이터 위치
  */
quint64 FileData::pos() const
{
    return pos_;
}
