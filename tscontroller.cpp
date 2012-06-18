#include "tscontroller.h"
#include "tscurvebuffer.h"
#include "tsreaderthread.h"
#include "tsusb3000reader.h"
TSController::TSController(TSIView *iview, TSModel *mod, QObject *parent) :
    QObject(parent), view(iview), model(mod)
{
}

void TSController::handle()
{
    TSReaderThread  *trd = new TSReaderThread(model->curveModel());
    trd->startRead();
    trd->stopRead();

    view->showGUI();
}
