#-------------------------------------------------
#
# libwmphoto 프로젝트
#
#-------------------------------------------------

include( ../common.pri )

TEMPLATE = lib

TARGET   = wmphoto

QT       -= gui

CONFIG   += staticlib

HEADERS  += \
	decode/decode.h \
	encode/encode.h \
	sys/ansi.h \
	sys/common.h \
	sys/perfTimer.h \
	sys/strTransform.h \
	sys/strcodec.h \
	sys/windowsmediaphoto.h \
	sys/xplatform_image.h

SOURCES  += \
	decode/WMPTranscode.c \
	decode/decode.c \
	decode/huffman.c \
	decode/postprocess.c \
	decode/segdec.c \
	decode/strInvTransform.c \
	decode/strPredQuantDec.c \
	decode/strdec.c \
	encode/encode.c \
	encode/segenc.c \
	encode/strFwdTransform.c \
	encode/strPredQuantEnc.c \
	encode/strenc.c \
	sys/adapthuff.c \
	sys/image.c \
	sys/perfTimerANSI.c \
	sys/strPredQuant.c \
	sys/strTransform.c \
	sys/strcodec.c

INCLUDEPATH += \
	../Network/client/debughlp/memtrace

QMAKE_CFLAGS_WARN_ON += -w
