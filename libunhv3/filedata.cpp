#include <QFile>
#include "filedata.hpp"

/** FileData 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in,
        FileData &fileData
        )
{
    uint len;
    fileData.pos_ = in.device()->pos();
    in >> fileData.FILE_;
    len = fileData.FILE_.chunkDataSize();

    // raw_data
    fileData.raw_data_pos = in.device()->pos();
    fileData.raw_data_len = len;
    fileData.fileStream_ = &in;

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
  @return 파일 데이터. delete 해줘야 합니다.
  */
QByteArray* FileData::raw_data() const
{
    char *data = nullptr;
    QByteArray *raw_data = nullptr;

    fileStream_->device()->seek(raw_data_pos);
    fileStream_->readBytes(data, const_cast<uint&>(raw_data_len));
    raw_data = new QByteArray(data, raw_data_len);
    delete data;

    return raw_data;
}

quint64 FileData::pos()
{
    return pos_;
}
