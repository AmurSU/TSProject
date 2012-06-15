#include "tsreaderthread.h"

TSReaderThread::TSReaderThread(TSCurveBuffer *b, QObject *parent) :QThread(parent){
    buffer = b;
    moveToThread(this);
}
void TSReaderThread::run(){
    TSUsb3000Reader *reader = new TSUsb3000Reader(this);
    reader->initDevice(buffer);
    reader->readData();
    emit done(reader);
    /*printf("%d",usbReader->readData());*/
    exec();
}
