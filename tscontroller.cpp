#include "tscontroller.h"
#include "tscurvebuffer.h"
#include <QTextCodec>
#include <QDebug>

TSController::TSController(TSIView *iview, TSModel *mod, QObject *parent) :
    QObject(parent), view(iview), model(mod)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    trd = new TSReaderThread(model->curveModel());
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
    trd->startRead();
    //trd->stopRead();
    view->startRecording();
}

void TSController::stopRecoringRequested()
{
    trd->stopRead();
    view->stopRecording();
}

void TSController::calibrateDialogRequested()
{
    view->showCalibrateionDialog(model->calibrateModel());
}

void TSController::calibrateDialogAccepted()
{
    model->curveModel()->setVolumeColibration(model->calibrateModel()->colibrateVolume);
    view->showCalibrateionDialog(model->calibrateModel());
}

void TSController::startVolumeCalibrationRequested()
{
    //view->showModelMessage(tr("Идет калибровка.\nПодождите"));
    TSUsb3000Reader *reader = new TSUsb3000Reader(this);
    reader->initDevice(0);
    int calibrate = reader->calibtateVolume();
    model->curveModel()->setVolumeColibration(calibrate);
    model->calibrateModel()->colibrateVolume = calibrate;
    //view->closeModelMessage();
    view->showCalibrateionDialog(model->calibrateModel());
    reader->closeReader();
    delete reader;
}

