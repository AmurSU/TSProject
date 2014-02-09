#-------------------------------------------------
#
# Project created by QtCreator 2012-06-15T02:30:09
#
#-------------------------------------------------
QT       += core\
            widgets\
            printsupport\
            sql

TARGET = TSProject
TEMPLATE = app

SOURCES += main.cpp\
    tsmodel.cpp \
    tscurvebuffer.cpp \
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
    tsrealcalc.cpp \
    tsrealcalcwrapper.cpp \
    tsusbdatareader.cpp \
    volumesolver.cpp

HEADERS  += \
    tsmodel.h \
    tscurvebuffer.h \
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
    tsrealcalc.h \
    tsrealcalcwrapper.h \
    tsusbdatareader.h \
    volumesolver.h

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
QMAKE_CXXFLAGS_RELEASE += -O2
