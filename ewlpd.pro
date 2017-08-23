QT += core websockets charts serialport

SOURCES += \
    main.cpp \
    lpdrpmsg.cpp \
    lpdgraphs.cpp \
    lpdserver.cpp

HEADERS += \
    lpdrpmsg.h \
    lpdgraphs.h \
    lpdserver.h

target.path = /usr/bin
INSTALLS += target

RESOURCES += \
    images.qrc
