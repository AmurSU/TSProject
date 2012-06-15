#ifndef TSREADERTHREAD_H
#define TSREADERTHREAD_H

#include <QThread>
#include "tsusb3000reader.h"
class TSReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit TSReaderThread(TSCurveBuffer *b,QObject *parent = 0);
    void run();
signals:
    void done(TSUsb3000Reader *reader);
public slots:
private:
    TSCurveBuffer *buffer;
};

#endif // TSREADERTHREAD_H
