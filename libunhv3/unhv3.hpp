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

#ifndef UNHV3_HPP
#define UNHV3_HPP

#include <QDir>
#include <QFile>
#include <QUuid>
#include <QString>
#include <QDateTime>
#include "filedatastorage.hpp"
#include "bondchunkheader.hpp"
#include "unhv3_global.hpp"
#include "fileinfolist.hpp"
#include "unhv3status.hpp"

class Unhv3Event;

/** hv3(꿀뷰 전용 포멧)을 풀어줍니다.\n
  이 클래스의 멤버 중 `[A-Z0-9]{4}_'형식으로 된 이름은 청크명, 청크 속성명입니다.\n
  <a href="http://www.kippler.com/doc/bond/BondFormat.txt">BondFormat Specification v1.0</a>를 참고하여 만들었습니다.\n
  고정된 청크 배치를 읽도록 하였으므로, 속성의 개수나 속성의 위치가 바뀐다면 소스를 수정해야 합니다.\n
  */
class UNHV3SHARED_EXPORT Unhv3
{
public:
    Unhv3();

    bool open(const QString &filepath);
    bool setEvent(Unhv3Event *event);

    // < -- 상태 확인 -- >
    bool testArchive();
    bool isOpened() const;
    bool isBrokenArchive();
    Unhv3Status lastError() const;
    const FileInfo* getFileItem(int index) const;

    // < -- 파일 정보 -- >
    int fileItemCount() const;
    uint formatVersion() const;
    uint archiveFileSize() const;
    QString filePathName() const;

    // < -- 압축 풀기 -- >
    bool extractAllTo(const QString &savePath);
    bool extractOneTo(int index, const QString &savePath);
    bool extractOneAs(int index, const QString &filePathName);

    // < -- 메타 정보 -- >
    uint      direction() const;
    uint      encryptMethod() const;
    QUuid     archiveUuid() const;
    QUuid     archiveGuid() const;
    QString   isbn() const;
    QString   genere() const;
    QString   comment() const;
    QString   publisher() const;
    QString   fileTitle() const;
    QString   fileMaker() const;
    QString   relatedLink() const;
    QString   originalWriter() const;
    QString   copyrightInformation() const;
    QString   originalPublishingDate() const;
    QDateTime createdTime() const;

protected:
    void clear();

private:
    Unhv3Event *event_;      ///< 이벤트 처리자.
    bool openStatus;         ///< 파일 열림 여부.
    QFile file;              ///< 파일 객체
    QDataStream fileStream_; ///< 파일 스트림
    Unhv3Status status;      ///< 객체 상태

    // < -- HV3 File Data -- >
    BondChunkHeader HV30_; ///< HV3 파일임을 의미
    uint            VERS_; ///< HV3 포맷의 버전 정보
    uint            FSIZ_; ///< 전체 파일의 크기
    BondChunkHeader HEAD_; ///< HV3 파일에 대한 정보
    QUuid           GUID_; ///< 파일의 GUID
    QUuid           UUID_; ///< UUID
    QDateTime       FTIM_; ///< 파일이 만들어진 시간
    uint            DIRE_; ///< 책의 제본방식 0: 정보없음, 1:left to right 2:right to left
    uint            ENCR_; ///< 파일의 암호화 방식, 0:암호화 없음
    QString         COPY_; ///< 저작권 정보
    QString         LINK_; ///< 관련 링크 URL
    QString         TITL_; ///< 파일의 제목
    QString         ISBN_; ///< 책의 ISBN 정보
    QString         WRTR_; ///< Original Writer
    QString         PUBL_; ///< Publisher
    QString         DATE_; ///< Original Publishing date
//  QString         COPY_; ///< Copyright Information
    QString         COMT_; ///< Comment
    QString         MAKR_; ///< HV3 File maker
    QString         GENR_; ///< Genere
    FileInfoList    LIST_; ///< 파일 정보 목록, 여러개의 하위 FINF 청크를 가진다.
    FileDataStorage BODY_; ///< 파일데이터를 담는 청크, 속성 청크는 가지지 않으며, 여러개의 하위 FILE 청크를 가진다.

};

#endif // UNHV3_HPP
