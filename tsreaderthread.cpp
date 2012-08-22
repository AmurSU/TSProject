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
    reader = new TSUsb3000Reader(this);
    reader->initDevice(buffer);
    switch (readingType)
    {
        case ReadAll:
        {
            while(this->ReadingStarted)
            {
                /*Debug()<<"It works";*/
                if (reader->readData()){
                    /*qDebug()<<"It works";*/
                    /*volume= buffer->volume();
                    tempIn = buffer->tempIn();
                    tempOut = buffer->tempOut();
                    end = buffer->end();*/
                    //qDebug()<<volume[end]<<tempIn[end]<<tempOut[end];
                    sleep(0.01);
                }
                else{
                    qDebug()<<"Get crashe";
                    sleep(0.01);
                }
            }
            break;
        }
        case ReadForVol:
        {
            int vol = reader->calibtateVolume();
            buffer->setVolumeColibration(vol);
            break;
        }
    }
    delete reader;
    emit done();
    exec();
}

void TSReaderThread::setReadingType(TSUsbReadingType type)
{
    readingType = type;
}

bool TSReaderThread::doWork()
{
    return works;
}
