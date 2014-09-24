#!/bin/bash

[ -e collect ] || exit

# COPYING
cp COPYING collect/

# hdp_image_plugin
mkdir collect/imageformats
cp hdp_image_plugin/Release/imageformats/libhdp_image.so collect/imageformats/

# libunhv3
cp -H libunhv3/Release/libunhv3.so collect/
cp libunhv3/unhv3_global.hpp collect/
cp libunhv3/unhv3/unhv3.hpp collect/
cp libunhv3/unhv3/unhv3status.hpp collect/
cp libunhv3/bondchunk/fileinfo.hpp collect/
cp libunhv3/bondchunk/bondchunkheader.hpp collect/

# libunhv3_include.pri
cat > collect/libunhv3_include.pri <<\EOF
#-------------------------------------------------
#
# libunhv3를 가져오기 위한 설정
#
#-------------------------------------------------

INCLUDEPATH *= $$PWD

EOF
