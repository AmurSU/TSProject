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
    /* if(reader!=0){
        delete reader;
        reader = 0;
    }*/
}


void TSReaderThread::run(){
    TSUsb3000Reader *reader = new TSUsb3000Reader(this);
    reader->initDevice(buffer);

    switch (readingType)
    {
    case ReadAll:
    {
        qDebug()<<"read all "<<this->ReadingStarted;
        while(this->ReadingStarted)
        {
            SHORT* adc;
            if ((adc=reader->readData())!=0){
                buffer->append(adc[0],adc[1],adc[2]);
                msleep(10);
            }
            else{
                qDebug()<<"Get crashe";
                msleep(10);
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
                qDebug()<<adc[0];
                if(i%10)
                    emit changeProgress(i/3);
                msleep(10);
            }
            else{
                qDebug()<<"Get crashe";
                msleep(10);
            }
            delete adc;
        }
        avg/=300;
        buffer->setVolumeColibration(avg,true);
        break;
    }
    case ReadForVolVal:
    {
        buffer->setEnd(0);
        for(int i=0;i<1200;i++)
        {
            SHORT* adc;
            if ((adc=reader->readData())!=0){
                buffer->append(adc[0],0,0);
                if(i%24)
                    emit changeProgress(i/12);
                msleep(10);
            }
            else{
                qDebug()<<"Get crashe";
                msleep(10);
            }
            delete adc;
        }

        break;
    }
    }
    ReadingStarted=false;
    qDebug()<<"WTF";
    if(reader!=0){
        delete reader;
    }
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
