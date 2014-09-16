#include "unhv3event.hpp"

void Unhv3Event::setError(
        const QString &filePathName,
        const Unhv3Status &status
        )
{
    Q_UNUSED(status)

    ARKERR nErr = 0;
    int t = 0;

    fileItem->isUnicodeFileName = true;
    fileItem->fileNameW = const_cast<WCHAR*>(filePathName.toStdWString().c_str());

    OnError(nErr, fileItem, 0, t);
}

void Unhv3Event::setProgress(
        float progress
        )
{
    int bStopAll = 0;
    int bStopCurrent = 0;
    progressInfo->fCurPercent = progress;

    OnProgressFile(progressInfo, bStopCurrent, bStopAll);
}

void Unhv3Event::setComplete()
{
    progressInfo->fTotPercent = 100;

    OnCompleteFile(progressInfo, 0);
}

Unhv3Event::~Unhv3Event()
{
    delete progressInfo;
    delete fileItem;
}

Unhv3Event::Unhv3Event()
{
    progressInfo = new SArkProgressInfo;
    fileItem = new SArkFileItem;
}
