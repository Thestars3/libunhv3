#-------------------------------------------------
#
# 공통된 컴파일 설정.
#
#-------------------------------------------------

#소스 디렉토리
PROJECT_DIR_PATH = $$PWD

INCLUDEPATH  += \
	$${PROJECT_DIR_PATH}/common/include \
	$${PROJECT_DIR_PATH}/image/sys \
	$${PROJECT_DIR_PATH}/image/x86 \
	$${PROJECT_DIR_PATH}/Systems/tools/WMPMetaLib

DEFINES     += __ANSI__

#Debug
CONFIG(debug, debug|release) {
	DEFINES += DEBUG

	DESTDIR = Debug

	QMAKE_CFLAGS_DEBUG += -O

#Release
} else {
	DEFINES += QT_NO_DEBUG_OUTPUT

	DESTDIR = Release

	QMAKE_CFLAGS_RELEASE += -O
}

UI_DIR      = $$DESTDIR

MOC_DIR     = $$DESTDIR

OBJECTS_DIR = $$DESTDIR
