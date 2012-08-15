#-------------------------------------------------
#
# Project created by QtCreator 2012-06-15T02:30:09
#
#-------------------------------------------------

QT       += core gui sql

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
    inputs/tscombobox.cpp \
    CRUDmw.cpp \
    bdgod.cpp \
    TSPatients.cpp \
    TSExaminations.cpp

HEADERS  += tsview.h \
    tscontroller.h \
    tsmodel.h \
    tsiview.h \
    tscurvebuffer.h \
    tsusb3000reader.h \
    tsreaderthread.h \
    inputs/tslineedit.h \
    inputs/tscombobox.h \
    ts_types.h \
    CRUDmw.h \
    bdgod.h \
    bdgod.h \
    TSPatients.h \
    TSExaminations.h
LIBS += -L"dll/*.dll"
FORMS    += \
    tsview.ui \
    patientprofile.ui \
    colibrateDialog.ui \
    CRUDmw.ui \
    dialog.ui

RESOURCES += \
    iconResourse.qrc \
    iconResourse.qrc

OTHER_FILES += \
    db_structure.sql
