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

/** pos 위치에서 시작하는 존재하는 파일 데이터 객체를 반환합니다.\n
  만약, 찾지 못한다면 nullptr를 반환합니다.
  @return 파일 데이터 객체 포인터.
  */
const FileData* FileDataStorage::getFileData(
        uint pos
        )
{
    foreach(FileData *fileData, fileDataStorage_) {
        if ( fileData->pos() == static_cast<quint64>(pos) ) {
            return fileData;
        }
    }
    return nullptr;
}
