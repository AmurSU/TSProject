#include "tscontroller.h"
#include "ts_types.h"
#include <QTextCodec>


TSController::TSController(TSIView *iview, TSModel *mod, QObject *parent) :
    QObject(parent), view(iview), model(mod)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
}

void TSController::handle()
{
    view->showGUI();
    //view->showRecordWindow(0);
}

void TSController::viewNewResearchRequested()
{
    TSPatientProfileModel* patientProfile = model->patientProfileModel();
    view->showNewResearchDialog(patientProfile);
}

void TSController::viewNewResearchAccepted()
{
    TSCurveBuffer *curve = model->curveModel();
    view->showRecordWindow(curve);
}

void TSController::listeningDeviceRequested()
{
    view->startRealtimeViewing();
}
