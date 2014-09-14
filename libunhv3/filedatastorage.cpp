#include "filedatastorage.hpp"

FileDataStorage::~FileDataStorage()
{
    qDeleteAll(fileDataStorage_);
}

/** FileDataStorage 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in,
        FileDataStorage &fileDataStorage
        )
{
    in >> fileDataStorage.BODY_;
    uint restSize = fileDataStorage.BODY_.subChunkSize();
    while ( restSize > 0 ) {
        FileData *fileData = new FileData();
        in >> *fileData;
        fileDataStorage.fileDataStorage_.push_back(fileData);
        restSize -= fileData->FILE().chunkDataSize() + BondChunkHeader::CHUNK_SIZE;
    }
    return in;
}
