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
                SHORT* adc;
                if ((adc=reader->readData())!=0){
                    /*qDebug()<<"It works";*/
                    /*volume= buffer->volume();
                    tempIn = buffer->tempIn();
                    tempOut = buffer->tempOut();
                    end = buffer->end();*/
                    //qDebug()<<volume[end]<<tempIn[end]<<tempOut[end];
                    buffer->append(adc[0],adc[1],adc[2]);
                    sleep(0.1);
                }
                else{
                    qDebug()<<"Get crashe";
                    sleep(0.1);
                }
                delete adc;
            }
            break;
        }
        case ReadForVolZer:
        {
            int avg=0;
            for(int i=0;i<300;i++)
            {
                SHORT* adc;
                if ((adc=reader->readData())!=0){
                    avg+=adc[0];
                    sleep(0.1);
                }
                else{
                    qDebug()<<"Get crashe";
                    sleep(0.1);
                }
                delete adc;
            }
            avg/=300;
            buffer->setVolumeColibration(avg);
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
