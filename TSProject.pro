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
    tsreaderthread.cpp \
    inputs/tslineedit.cpp \
    inputs/tscombobox.cpp

HEADERS  += tsview.h \
    tscontroller.h \
    tsmodel.h \
    tsiview.h \
    tscurvebuffer.h \
    tsusb3000reader.h \
    tsreaderthread.h \
    inputs/tslineedit.h \
    inputs/tscombobox.h \
    ts_types.h

FORMS    += \
    tsview.ui \
    patientprofile.ui

RESOURCES += \
    iconResourse.qrc \
    iconResourse.qrc





















