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

#include <JXRTest.h>
#include <QByteArray>

class HdpConverter
{
private:
    typedef struct tagWMPDECAPPARGS
    {
        char* szInputFile;
        char* szOutputFile;

        Bool bVerbose;

        PKPixelFormatGUID guidPixFormat;

        // region decode
        size_t rLeftX;
        size_t rTopY;
        size_t rWidth;
        size_t rHeight;

        // thumbnail
        size_t tThumbnailFactor;

        // orientation
        ORIENTATION oOrientation;

        // post processing
        U8 cPostProcStrength;

        U8 uAlphaMode; // 0:no alpha 1: alpha only else: something + alpha

        SUBBAND sbSubband;  // which subbands to keep (for transcoding)

        BITSTREAMFORMAT bfBitstreamFormat; // desired bitsream format (for transcoding)

        CWMIStrCodecParam wmiSCP;

        Bool bIgnoreOverlap;
    } WMPDECAPPARGS;
    void WmpDecAppCreateEncoderFromExt(PKCodecFactory* pCFactory, const char* szExt, PKImageEncode** ppIE);
    void WmpDecAppParseArgs(WMPDECAPPARGS* args);
    void WmpDecAppInitDefaultArgs(WMPDECAPPARGS* args);

public:
    QByteArray convertToBmp();

};

#endif // HDPCONVERTER_HPP
