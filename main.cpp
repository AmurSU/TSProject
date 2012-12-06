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
try{
    TSController* controller = new TSController;


        controller->showMaximized();
    }catch(TSController* controller){
        qDebug()<<"Контролера сламалася";
    }

    return a.exec();
}
