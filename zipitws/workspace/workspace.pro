TARGET = zipitws-workspace
DESTDIR = ../bin
SOURCES += main.cpp \
    mainpanel.cpp \
    mouseemulator.cpp
FORMS += mainpanel.ui
HEADERS += mainpanel.h \
    mouseemulator.h
include(../zipit.pri)
