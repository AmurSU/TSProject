#-------------------------------------------------
#
# Project created by QtCreator 2012-06-15T02:30:09
#
#-------------------------------------------------

QT       += core gui

TARGET = TSProject
TEMPLATE = app


SOURCES += main.cpp\
        tsview.cpp \
    tscontroller.cpp \
    tsmodel.cpp \
    tscurvebuffer.cpp \
    tsusb3000reader.cpp \
    tsreaderthread.cpp

HEADERS  += tsview.h \
    tscontroller.h \
    tsmodel.h \
    tsiview.h \
    tscurvebuffer.h \
    tsusb3000reader.h \
    tsreaderthread.h
LIBS += -L"dll/*.dll"
FORMS    += \
    tsview.ui

RESOURCES += \
    iconResourse.qrc \
    iconResourse.qrc

















