#-------------------------------------------------
#
# libunhv3 프로젝트
#
#-------------------------------------------------

include( common.pri )

TEMPLATE = lib

TARGET   = unhv3

QT       -= gui

VERSION  = $$system(cat $${ROOT_PATH}/version)

HEADERS  += \
	unhv3.hpp \
	unhv3_global.hpp \
    bondchunkheader.hpp \
    bondchunkattr.hpp \
    unhv3status.hpp \
    fileinfo.hpp \
    fileinfolist.hpp \
    bondchunk.hpp \
    ufp.hpp

SOURCES  += \
	unhv3.cpp \
    bondchunkheader.cpp \
    bondchunkattr.cpp \
    fileinfo.cpp \
    fileinfolist.cpp \
    bondchunk.cpp \
    ufp.cpp

DEFINES  += \
	UNHV3_LIBRARY

QMAKE_CXXFLAGS += -std=c++0x
