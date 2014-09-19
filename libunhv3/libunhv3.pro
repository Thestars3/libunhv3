#-------------------------------------------------
#
# libunhv3 프로젝트
#
#-------------------------------------------------

include( ../common.pri )

include( $$ROOT_PATH/hdp_image_plugin/hdp_image_plugin_include.pri )

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

DEFINES  += UNHV3_LIBRARY

QMAKE_CXXFLAGS += -std=c++0x
