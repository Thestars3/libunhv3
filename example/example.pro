include( ../common.pri )

QT       += core

TARGET = untitled

CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../libunhv3/unhv3 ../libunhv3 ../libunhv3/bondchunk

LIBS += -L../libunhv3/$$DESTDIR -lunhv3
