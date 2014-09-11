#-------------------------------------------------
#
# libwmglue 프로젝트
#
#-------------------------------------------------

include( ../../common.pri )

TEMPLATE = lib

TARGET   = wmglue

QT       -= gui

CONFIG   += staticlib

HEADERS  += \
	WMPGlueLib/WMPGlue.h \
	WMPMetaLib/WMPMeta.h

SOURCES  += \
	WMPGlueLib/WMPGlue.c \
	WMPGlueLib/WMPGlueBmp.c \
	WMPGlueLib/WMPGlueHdr.c \
	WMPGlueLib/WMPGluePFC.c \
	WMPGlueLib/WMPGluePnm.c \
	WMPGlueLib/WMPGlueTif.c \
	WMPGlueLib/WMPGlueWmp.c \
	WMPGlueLib/WMPGlueYUV.c \
	WMPMetaLib/WMPMeta.c

INCLUDEPATH += \
	WMPGlueLib

QMAKE_CFLAGS_WARN_ON += -w
