#include "tsreaderthread.h"
#include "conio.h"

TSReaderThread::TSReaderThread(TSCurveBuffer *b, QObject *parent) :QThread(parent){
    buffer = b;
    moveToThread(this);
}
void TSReaderThread::f1 (){
    qDebug()<<"-----Thread is open for command----";
}
void TSReaderThread::startRead (){
    ReadingStarted = true;
    this->start();
}
void TSReaderThread::stopRead (){
    ReadingStarted=false;
    this->terminate();
}


void TSReaderThread::run(){
    TSUsb3000Reader *reader = new TSUsb3000Reader(this);
    reader->initDevice(buffer);
    int* volume= buffer->volume();
    int* tempIn = buffer->tempIn();
    int* tempOut = buffer->tempOut();
    int end = buffer->end();
    while(this->ReadingStarted){
        if (reader->readData()){
            /*qDebug()<<"It works";*/
            volume= buffer->volume();
            tempIn = buffer->tempIn();
            tempOut = buffer->tempOut();
            end = buffer->end();
            /*qDebug()<<volume[end]<<tempIn[end]<<tempOut[end];*/
            sleep(0.007);
        }
        else
            qDebug()<<"Get crashe";
    }
    emit done(reader);
    exec();
}
