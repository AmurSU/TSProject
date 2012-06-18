#include "tscontroller.h"
#include <QTextCodec>

TSController::TSController(TSIView *iview, TSModel *mod, QObject *parent) :
    QObject(parent), view(iview), model(mod)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
}

void TSController::handle()
{
    view->showGUI();
}

void TSController::newResearchRequesdted()
{
    TSPatientProfileModel *m = model->newResearchModel();
    view->showNewResearchDialog(m);
}

void TSController::editPatientProfileRequested()
{
    TSPatientProfileModel *m = model->newResearchModel();
    view->showEditPatientProfileDialog(m);
}

void TSController::newResearchAccepted()
{
    view->showResearchwindow(model->curveModel());
}

void TSController::startRecordingRequested()
{
    view->startRecording();
}
