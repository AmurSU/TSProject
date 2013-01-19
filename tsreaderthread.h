#ifndef TSREADERTHREAD_H
#define TSREADERTHREAD_H

#include <QThread>
#include "tsusb3000reader.h"
#include "extremum.h"

class TSReaderThread : public QObject
{
    Q_OBJECT
public:
    explicit TSReaderThread(TSCurveBuffer *b,QObject *parent = 0);
    void run();
    void stopRead();
    void startRead();
    void setReadingType(TSUsbReadingType type);
    bool doWork();
    void readd();
    TSUsb3000Reader* getReader();
signals:
    void done();
    void changeProgress(int value);
public slots:
    void trd_done();
    void trd_progress(int value);
private:
    TSCurveBuffer *buffer;
    bool ReadingStarted;
    TSUsbReadingType readingType;
    bool works;
    TSUsb3000Reader *reader;
};

#endif // TSREADERTHREAD_H
