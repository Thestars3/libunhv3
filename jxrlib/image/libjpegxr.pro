#-------------------------------------------------
#
# libjpegxr 프로젝트
#
#-------------------------------------------------

include( ../common.pri )

TEMPLATE = lib

TARGET   = jpegxr

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
	decode/JXRTranscode.c \
	decode/decode.c \
	decode/postprocess.c \
	decode/segdec.c \
	decode/strInvTransform.c \
	decode/strPredQuantDec.c \
	decode/strdec.c \
	decode/strdec_x86.c \
	encode/encode.c \
	encode/segenc.c \
	encode/strFwdTransform.c \
	encode/strPredQuantEnc.c \
	encode/strenc.c \
	encode/strenc_x86.c \
	sys/adapthuff.c \
	sys/image.c \
	sys/perfTimerANSI.c \
	sys/strPredQuant.c \
	sys/strTransform.c \
	sys/strcodec.c
