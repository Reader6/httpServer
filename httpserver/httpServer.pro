TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += \
    commen.cpp \
    easylogging++.cc \
    httpServer.cpp \
    http_conn.cpp \
        locker.cpp \
    main.cpp

HEADERS += \
    commen.h \
    easylogging++.h \
    httpServer.h \
    http_conn.h \
    locker.h \
    threadpool.h\
QMAKE_CFLAGS  += -lpthread
LIBS +=-lpthread


DISTFILES += \
    my_log.conf \
    var/www/index.html
