#include "tscontroller.h"

TSController::TSController(TSIView *iview, TSModel *mod, QObject *parent) :
    QObject(parent), view(iview), model(mod)
{
}

void TSController::handle()
{
    qDebug()<<"Hellow georg";
    qDebug()<<"Say hallow!";
    view->showGUI();
}
