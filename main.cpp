#include <QtGui/QApplication>
#include "tsmodel.h"
#include <tscontroller.h>
#include <QRegExp>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TSController* controller = new TSController;
    controller->show();

    return a.exec();
}
