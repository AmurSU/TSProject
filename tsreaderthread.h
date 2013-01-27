#ifndef TSREADERTHREAD_H
#define TSREADERTHREAD_H

#include <QThread>
#include "tsusb3000reader.h"

enum TSUsbReadingType {ReadAll,ReadForVolZer,ReadForVolVal,ReadForTemp};

class TSReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit TSReaderThread(TSCurveBuffer *b,QObject *parent = 0);
    void run();
    void f1();
    void stopRead();
    void startRead();
    void setReadingType(TSUsbReadingType type);
    bool doWork();
signals:
    void done();
    void changeProgress(int value);
public slots:
private:
    TSCurveBuffer *buffer;
    bool ReadingStarted;
    TSUsbReadingType readingType;
    bool works;
    //TSUsb3000Reader *reader;
};

#endif // TSREADERTHREAD_H
