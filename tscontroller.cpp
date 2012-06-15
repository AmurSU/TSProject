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
    /*TSUsb3000Reader *tur = new TSUsb3000Reader();
    tur->initDevice();
    tur->moveToThread(trd);
    trd->run(tur);*/

    view->showGUI();
}
