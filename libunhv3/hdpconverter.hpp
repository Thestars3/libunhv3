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
#include <QByteArray>

/** HDP 변환기.\n
  HDP -> JPEG, HDP -> PNG 단방향 포멧 변환 기능을 제공합니다.\n
  JPEG 변환시, 이미지는 압축되지 않습니다.\n
  @warning 변환 결과물은 모든 메타 정보를 소실합니다.
  */
class HdpConverter
{
private:
    static HdpConverter *singleton;
    QImage image;
    static QByteArray imageRawData;
    HdpConverter();
    ~HdpConverter();
    void initConverter(const QByteArray &hdpData);
    friend ERR PKImageEncode_WritePixels_QImage(PKImageEncode *pIE, U32 cLine, U8 *pbPixel, U32 cbStride);
    static ERR PKCodecFactory_CreateDecoderFromMemory(const QByteArray &source, PKImageDecode **ppDecoder);
    static ERR WriteQImageHeader(PKImageEncode *pIE, QImage &image);
    static QByteArray* convertRgbaToArgb(uchar *rgbaData, uint size);

    /** inch 단위를 미터 단위로 바꿉니다.
      @return 미터 단위로된 값
      */
    static inline float inchConvertToMeter(
            const float &inch ///< 인치 단위
            )
    {
         // inch * 5000 / 127 = meter
        return inch * ( 100 / 2.54 );
    }

public:
    void relrease();
    bool hasAlphaChannel();
    HdpConverter* getInstance();
    HdpConverter* setData(const QByteArray &hdpData);
    bool saveToJpeg(const QString &filePath);
    bool saveToPng(const QString &filePath);

};

ERR PKImageEncode_WritePixels_QImage(PKImageEncode *pIE, U32 cLine, U8 *pbPixel, U32 cbStride);

#endif // HDPCONVERTER_HPP
