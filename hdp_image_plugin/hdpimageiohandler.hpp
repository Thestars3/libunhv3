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

#ifndef HDPIMAGEIOHANDLER_HPP
#define HDPIMAGEIOHANDLER_HPP

#include <QByteArray>
#include <QImageIOHandler>

typedef struct tagPKImageEncode PKImageEncode;
typedef struct tagPKImageDecode PKImageDecode;
typedef struct tagPKFormatConverter PKFormatConverter;
typedef struct tagPKRect PKRect;
typedef uint U32;
typedef uchar U8;
typedef long ERR;

class HdpImageIOHandler :
        public QImageIOHandler
{
public:
    virtual bool canRead() const;
    virtual bool read(QImage *outImage);

private:
    ERR PKImageEncode_Transcode(PKImageEncode *pIE, PKFormatConverter *pFC, PKRect *pRect, QImage *outImage);
    ERR PKImageEncode_WritePixels_QImage(PKImageEncode *pIE, U32 cLine, U8 *pbPixel, U32 cbStride, QImage *outImage);
    ERR PKCodecFactory_CreateDecoderFromMemory(PKImageDecode **ppDecoder);
    ERR WriteQImageHeader(PKImageEncode *pIE, QImage *image);
    void convertRgbaToArgb(uchar *data, uint size);

    /** inch 단위를 미터 단위로 바꿉니다.
      @return 미터 단위로된 값
      */
    template<typename T> static inline T inchConvertToMeter(
            const T &inch ///< 인치 단위
            )
    {
         // inch * 5000 / 127 = meter
        return inch * ( 100 / 2.54 );
    }

};

#endif // HDPIMAGEIOHANDLER_HPP
