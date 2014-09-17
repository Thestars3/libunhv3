#-------------------------------------------------
#
# jxrlib를 가져오기 위한 설정
#
#-------------------------------------------------

#소스 디렉토리
PROJECT_DIR_PATH = $$PWD

INCLUDEPATH += \
	$${PROJECT_DIR_PATH}/jxrgluelib \
	$${PROJECT_DIR_PATH}/jxrtestlib \
	$${PROJECT_DIR_PATH}/image/sys \
	$${PROJECT_DIR_PATH}/common/include

DEFINES += \
	__ANSI__ \
	DISABLE_PERF_MEASUREMENT

LIBS    += \
	-lm \
	-static \
	-L'$${PROJECT_DIR_PATH}/$$DESTDIR' -ljxrglue \
	-L'$${PROJECT_DIR_PATH}/image/$$DESTDIR' -ljpegxr

#PRE_TARGETDEPS += $${PROJECT_DIR_PATH}/$$DESTDIR/libjxrglue.a $${PROJECT_DIR_PATH}/image/$$DESTDIR/libjpegxr.a
