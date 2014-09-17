#-------------------------------------------------
#
# libunhv3 프로젝트
#
#-------------------------------------------------

include( ../common.pri )

include( bondchunk/bondchunk.pri )

include( unhv3/unhv3.pri )

TEMPLATE = lib

TARGET   = unhv3

QT       += gui

VERSION  = $$system(cat $${ROOT_PATH}/version)

HEADERS  += \
	ufp.hpp

SOURCES  += \
	ufp.cpp

QTPLUGIN += hdp_image

DEFINES  += UNHV3_LIBRARY

QMAKE_LIBDIR   += $$ROOT_PATH/hdp_image_plugin/$$DESTDIR

QMAKE_CXXFLAGS += -std=c++0x
