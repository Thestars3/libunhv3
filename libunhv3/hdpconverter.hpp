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

#ifndef HDPCONVERTER_HPP
#define HDPCONVERTER_HPP

#include <QImage>
#include <JXRTest.h>
#include <QByteArray>

class HdpConverter
{
private:
    char *pExt;
    PKImageDecode *pDecoder;
    void WmpDecAppCreateEncoder(PKImageEncode **ppIE);
    ERR PKCodecFactory_CreateDecoderFromMemory(const QByteArray &source);

public:
    HdpConverter();
    QByteArray convertToBmp(const QByteArray &source);

};

#endif // HDPCONVERTER_HPP
