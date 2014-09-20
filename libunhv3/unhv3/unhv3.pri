
INCLUDEPATH *= $$PWD

HEADERS  += \
    $$PWD/unhv3.hpp \
    $$PWD/unhv3_global.hpp \
    $$PWD/unhv3event.hpp \
    $$PWD/unhv3status.hpp \ 
	$$PWD/EventInterface.hpp \
    unhv3/arkerrconverter.hpp

SOURCES  += \
    $$PWD/unhv3.cpp \
    $$PWD/unhv3event.cpp \
    $$PWD/unhv3status.cpp \ 
    unhv3/arkerrconverter.cpp
