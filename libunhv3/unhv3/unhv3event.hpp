/*Copyright (C) 2014  별님

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef UNHV3EVENT_HPP
#define UNHV3EVENT_HPP

#include "unhv3status.hpp"

typedef int ARKERR;
typedef int ARK_OVERWRITE_MODE;
typedef int ARK_COMPRESSION_METHOD;
typedef int ARK_ENCRYPTION_METHOD;
typedef uint ARK_FILEATTR;
typedef qint64 INT64;
typedef qint32 BOOL32;
typedef quint32 UINT32;
typedef UINT32 ARK_TIME_T;
typedef wchar_t WCHAR;
typedef const wchar_t* LPCWSTR;
typedef char CHAR;
struct SArkNtfsFileTimes;

struct SArkProgressInfo {
    float   fCurPercent;
    float   fTotPercent;
    int     bCompleting;
    float   fCompletingPercent;
};

struct SArkFileItem {
    CHAR*                   fileName;
    WCHAR*                  fileNameW;
    WCHAR*                  fileCommentW;
    ARK_TIME_T              fileTime;
    INT64                   compressedSize;
    INT64                   uncompressedSize;
    ARK_ENCRYPTION_METHOD   encryptionMethod;
    ARK_FILEATTR            attrib;
    UINT32                  crc32;
    ARK_COMPRESSION_METHOD  compressionMethod;
    SArkNtfsFileTimes*      ntfsFileTimes;
    BOOL32                  isUnicodeFileName;
};

#define ARK_MAX_PATH 300

class Unhv3Event
{
public:
    void setOpen();
    void setStartFile(const QString &filePath);
    void setError(const QString &filePathName, const Unhv3Status &status);
    void setProgress(float progress);
    void setComplete();
    QString convertDuplicatedName(const QString &filePath);

    virtual void OnOpening(const SArkFileItem *pFileItem, float progress, BOOL32 &bStop) = 0;
    virtual void OnStartFile(const SArkFileItem *pFileItem, BOOL32 &bStopCurrent, BOOL32 &bStopAll, int index) = 0;
    virtual void OnProgressFile(const SArkProgressInfo *pProgressInfo, BOOL32 &bStopCurrent, BOOL32 &bStopAll) = 0;
    virtual void OnCompleteFile(const SArkProgressInfo *pProgressInfo, ARKERR nErr) = 0;
    virtual void OnError(ARKERR nErr, const SArkFileItem *pFileItem, BOOL32 bIsWarning, BOOL32 &bStopAll) = 0;
    virtual void OnAskOverwrite(const SArkFileItem *pFileItem, LPCWSTR szLocalPathName, ARK_OVERWRITE_MODE &overwrite, WCHAR pathName2Rename[ARK_MAX_PATH]) = 0;

};

#endif // UNHV3EVENT_HPP
