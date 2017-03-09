QT += charts serialport

SOURCES += \
    main.cpp \
    lpdrpmsg.cpp \
    lpdgraphs.cpp

HEADERS += \
    lpdrpmsg.h \
    lpdgraphs.h

target.path = /usr/bin
INSTALLS += target

RESOURCES += \
    images.qrc
