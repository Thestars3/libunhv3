#-------------------------------------------------
#
# 공통된 컴파일 설정.
#
#-------------------------------------------------

#소스 디렉토리
PROJECT_DIR_PATH = $$PWD

VERSION = 1.1

INCLUDEPATH  += \
	$${PROJECT_DIR_PATH}/common/include \
	$${PROJECT_DIR_PATH}/image/sys \
	$${PROJECT_DIR_PATH}/image/x86

DEFINES      += \
	__ANSI__ \
	DISABLE_PERF_MEASUREMENT

QMAKE_CFLAGS_WARN_ON += -w

#Debug
CONFIG(debug, debug|release) {
	DEFINES += DEBUG

	DESTDIR = Debug

#Release
} else {
	DEFINES += QT_NO_DEBUG_OUTPUT

	DESTDIR = Release

	QMAKE_CFLAGS_RELEASE += -O
}

UI_DIR      = $$DESTDIR

MOC_DIR     = $$DESTDIR

OBJECTS_DIR = $$DESTDIR
