#-------------------------------------------------
#
# Project created by QtCreator 2012-06-15T02:30:09
#
#-------------------------------------------------
QT       += core gui\
            sql

TARGET = TSProject
TEMPLATE = app

SOURCES += main.cpp\
    tsmodel.cpp \
    tscurvebuffer.cpp \
    tsusb3000reader.cpp \
    tsreaderthread.cpp \
    inputs/tslineedit.cpp \
    inputs/tscombobox.cpp \
    tscontroller.cpp \
    tools/tsvalidationtools.cpp\
    CRUDmw.cpp \
    bdgod.cpp \
    models/TSPatients.cpp \
    models/TSExaminations.cpp \
    tsanalitics.cpp \
    inputs/tstableview.cpp \
    tstempanalitic.cpp \
    tsusbdatareader.cpp

HEADERS  += \
    tsmodel.h \
    tscurvebuffer.h \
    tsusb3000reader.h \
    tsreaderthread.h \
    inputs/tslineedit.h \
    inputs/tscombobox.h \
    ts_types.h \
    tscontroller.h \
    tools/tsvalidationtools.h\
    CRUDmw.h \
    bdgod.h \
    bdgod.h \
    models/TSPatients.h \
    models/TSExaminations.h \
    tsanalitics.h \
    inputs/tstableview.h \
    ui_tsprintview.h \
    tstempanalitic.h \
    extremum.h \
    tsusbdatareader.h

LIBS += -L"dll/*.dll"
FORMS    += \
    patientprofile.ui \
    colibrateDialog.ui \
    tsview.ui\
    CRUDmw.ui \
    dialog.ui \
    tsresultsview.ui \
    tsvolsignalwidget.ui \
    tsprintview.ui

RESOURCES += \
    iconResourse.qrc

OTHER_FILES += \
    db_structure.sql
