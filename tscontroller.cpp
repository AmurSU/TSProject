#include "tscontroller.h"
#include "tscurvebuffer.h"
#include "tsreaderthread.h"
#include "tsusb3000reader.h"
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
    TSReaderThread  *trd = new TSReaderThread(model->curveModel());
    trd->startRead();
    // trd->stopRead();
    view->startRecording();
}
