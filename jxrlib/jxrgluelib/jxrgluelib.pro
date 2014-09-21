#-------------------------------------------------
#
# jxrgluelib 프로젝트
#
#-------------------------------------------------

include( ../common.pri )

TEMPLATE = lib

TARGET   = jxrglue

QT       -= gui

CONFIG   += staticlib

HEADERS  += \
	JXRGlue.h \
	JXRMeta.h

SOURCES  += \
	JXRGlue.c \
	JXRGlueJxr.c \
	JXRGluePFC.c \
	JXRMeta.c