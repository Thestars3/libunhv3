#-------------------------------------------------
#
# libunhv3 프로젝트
#
#-------------------------------------------------

include( ../common.pri )

include( $$ROOT_PATH/hdp_image_plugin/hdp_image_plugin_include.pri )

TEMPLATE = lib

TARGET   = unhv3

QT       += gui

VERSION  = $$system(cat $${ROOT_PATH}/version)

HEADERS  += \
	unhv3.hpp \
	unhv3_global.hpp \
    bondchunkheader.hpp \
    bondchunkattr.hpp \
    unhv3status.hpp \
    fileinfo.hpp \
    fileinfolist.hpp \
    ufp.hpp \
    filedatastorage.hpp \
    filedata.hpp

SOURCES  += \
	unhv3.cpp \
    bondchunkheader.cpp \
    bondchunkattr.cpp \
    fileinfo.cpp \
    fileinfolist.cpp \
    ufp.cpp \
    filedatastorage.cpp \
    filedata.cpp \
    unhv3status.cpp

DEFINES  += \
	UNHV3_LIBRARY

QTPLUGIN += hdp_image

QMAKE_CXXFLAGS += -std=c++0x
