#include "tscontroller.h"
#include "tscurvebuffer.h"
#include "tsanalitics.h"
#include <QTextCodec>
#include <QDebug>
#include <QVector>
#include <qmath.h>
TSController::TSController(TSIView *iview, TSModel *mod, QObject *parent) :
    QObject(parent), view(iview), model(mod)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    trd = new TSReaderThread(model->curveModel());
}

void TSController::handle()
{
    tsanalitics *ga = new tsanalitics();
    int i=0;
    FILE *in;
    in=fopen("inp.txt","r");
    int n=0,t=0,t1,t2;
    fscanf(in,"%d",&n);
    for(i=0;i<n;i++){
       fscanf(in,"%d %d %d",&t1,&t2,&t);
       ga->append(t);
    }
    fclose(in);
    ga->findExtremums();
    for(i=0;i<1;i++){
    ga->deleteBadExtremums();
    qDebug()<<"-------------------------------";
    }
    qDebug()<<ga->getAvgExpiratory();
    qDebug()<<ga->getAvgInspiratory();
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

