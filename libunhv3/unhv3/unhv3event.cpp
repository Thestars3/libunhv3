#include <QFileInfo>
#include "unhv3event.hpp"

void Unhv3Event::setOpen()
{
    BOOL32 bStop;
    OnOpening(nullptr, 0, bStop);
}

void Unhv3Event::setStartFile(
        const QString &filePath
        )
{
    BOOL32 b;
    SArkFileItem fileItem;

    fileItem.isUnicodeFileName = true;
    fileItem.fileNameW = const_cast<WCHAR*>(filePath.toStdWString().c_str());

    OnStartFile(&fileItem, b, b, 0);
}

QString Unhv3Event::convertDuplicatedName(
        const QString &filePath
        )
{
    LPCWSTR szLocalPathName = QFileInfo(filePath).absolutePath().toStdWString().c_str();
    ARK_OVERWRITE_MODE overwrite;
    WCHAR pathName2Rename[ARK_MAX_PATH];

    OnAskOverwrite(nullptr, szLocalPathName, overwrite, pathName2Rename);

    return QString::fromWCharArray(pathName2Rename);
}

void Unhv3Event::setError(
        const QString &filePathName,
        const Unhv3Status &status
        )
{
    Q_UNUSED(status)

    SArkFileItem fileItem;
    ARKERR nErr = 0;
    BOOL32 b;

    fileItem.isUnicodeFileName = true;
    fileItem.fileNameW = const_cast<WCHAR*>(filePathName.toStdWString().c_str());

    OnError(nErr, &fileItem, 0, b);
}

void Unhv3Event::setProgress(
        float progress
        )
{
    BOOL32 b;
    SArkProgressInfo progressInfo;

    progressInfo.fCurPercent = progress;

    OnProgressFile(&progressInfo, b, b);
}

void Unhv3Event::setComplete()
{
    SArkProgressInfo progressInfo;

    progressInfo.fTotPercent = 100;

    OnCompleteFile(&progressInfo, 0);
}
