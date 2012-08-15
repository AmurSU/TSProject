#include <QtGui/QApplication>
#include <QTextCodec>
#include "CRUDmw.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();


}
