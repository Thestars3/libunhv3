#-------------------------------------------------
#
# jxrlib를 가져오기 위한 설정
#
#-------------------------------------------------

INCLUDEPATH += \
	$${PWD}/jxrgluelib \
	$${PWD}/image/sys \
	$${PWD}/common/include

DEFINES     += \
	__ANSI__ \
	DISABLE_PERF_MEASUREMENT

LIBS        += \
	-lm \
	-L'$${PWD}/jxrgluelib/$$DESTDIR' -ljxrglue \
	-L'$${PWD}/image/$$DESTDIR' -ljpegxr
