#include <QtGui/QApplication>
#include "tsview.h"
#include "tsmodel.h"
#include "tscontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TSView *view = new TSView;
    TSModel *model = new TSModel;
    TSController *controller = new TSController(view,model);

    view->setController(controller);
    controller->handle();

    return a.exec();
}
