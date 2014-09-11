#-------------------------------------------------
#
# libunhv3 프로젝트
#
#-------------------------------------------------

include( common.pri )

include( ../jxrlib/jxrlib_include.pri )

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
    filedata.hpp \
    hdpconverter.hpp

SOURCES  += \
	unhv3.cpp \
    bondchunkheader.cpp \
    bondchunkattr.cpp \
    fileinfo.cpp \
    fileinfolist.cpp \
    ufp.cpp \
    filedatastorage.cpp \
    filedata.cpp \
    hdpconverter.cpp

DEFINES  += \
	UNHV3_LIBRARY

QMAKE_CXXFLAGS += -std=c++0x
