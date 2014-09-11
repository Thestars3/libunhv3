#include <QVectorIterator>
#include "fileinfo.hpp"
#include "fileinfolist.hpp"

/** FileInfoList 역직렬화 수행자.
  */
QDataStream& operator>>(
        QDataStream &in,
        FileInfoList &fileInfoList
        )
{
    in >> fileInfoList.LIST_;
    uint restSize = fileInfoList.LIST_.subChunkSize();
    while ( restSize > 0 ) {
        FileInfo *fileInfo = new FileInfo();
        in >> *fileInfo;
        fileInfoList.fileInfoList_.push_back(fileInfo);
        restSize -= fileInfo->FINF().subChunkSize() + BondChunkHeader::CHUNK_SIZE;
    }
    return in;
}

FileInfoList::~FileInfoList()
{
    for(QVector<FileInfo*>::Iterator it = fileInfoList_.end() - 1; it == fileInfoList_.begin(); it--) {
        delete *it;
    }
}