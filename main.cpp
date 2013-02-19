#include <QtGui/QApplication>
#include "tsmodel.h"
#include <tscontroller.h>
#include <QRegExp>
#include <QDebug>
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TSController* controller = new TSController;

    //setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    //controller->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint|Qt::SubWindow);
    controller->showMaximized();
    return a.exec();
}
