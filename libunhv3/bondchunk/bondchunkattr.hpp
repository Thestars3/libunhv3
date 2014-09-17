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

#ifndef BONDCHUNKATTR_HPP
#define BONDCHUNKATTR_HPP

#include <QUuid>
#include <QDateTime>
#include <QByteArray>
#include <QDataStream>

/** 속성 청크
  */
class BondChunkAttr
{
private:
    const static class QTextCodec *textCodec; /// 텍스트 변환 코덱
    QString attrName_;       ///< 속성의 이름.
    QByteArray attrData_;    ///< 속성 데이터.
    friend QDataStream& operator>>(QDataStream &in, BondChunkAttr &bondChunkAttr);

public:
    BondChunkAttr(const QString &attrName);

    // < -- Getter -- >
    QString attrName() const;
    QByteArray attrData() const;

    // < -- 변환 메소드 -- >
    QString fromString() const;
    QDateTime fromFiletime() const;
    uint fromDword() const;
    QUuid fromGuid() const;
    QUuid fromUuid() const;

};

QDataStream& operator>>(QDataStream &in, BondChunkAttr &bondChunkAttr);

#endif // BONDCHUNKATTR_HPP
