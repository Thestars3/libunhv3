#-------------------------------------------------
#
# libjxrglue 프로젝트
#
#-------------------------------------------------

include( common.pri )

TEMPLATE = lib

TARGET   = jxrglue

QT       -= gui

CONFIG   += staticlib

HEADERS  += \
	jxrgluelib/JXRGlue.h \
	jxrgluelib/JXRMeta.h \
	jxrtestlib/JXRTest.h

SOURCES  += \
	jxrgluelib/JXRGlue.c \
	jxrgluelib/JXRGlueJxr.c \
	jxrgluelib/JXRGluePFC.c \
	jxrgluelib/JXRMeta.c \
	jxrtestlib/JXRTest.c \
	jxrtestlib/JXRTestBmp.c \
	jxrtestlib/JXRTestHdr.c \
	jxrtestlib/JXRTestPnm.c \
	jxrtestlib/JXRTestTif.c \
	jxrtestlib/JXRTestYUV.c

INCLUDEPATH += \
	jxrgluelib \
	jxrtestlib
