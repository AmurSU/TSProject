#include "tsreaderthread.h"
#include "conio.h"

TSReaderThread::TSReaderThread(TSCurveBuffer *b, QObject *parent) :QThread(parent){
    buffer = b;
    moveToThread(this);
    reader = 0;
}


void TSReaderThread::startRead (){
    ReadingStarted = true;
    reader = new TSUsb3000Reader(this);
    reader->initDevice(buffer);
    this->start();
}
void TSReaderThread::stopRead (){
    if(ReadingStarted){
        ReadingStarted=false;
        //reader->closeReader();

        //this->terminate();
    }
    qDebug()<<"RSF";
//    if(reader!=0){
//        delete reader;
//        reader = 0;
//    }
 /*   if (!this->wait()){
        qDebug()<<"Waiting!!!!!!!!!";
        if(reader!=0){
            delete reader;
            reader = 0;
        }
        this->quit();
    }*/


}


void TSReaderThread::run(){

    qDebug()<<"thread started run";
    switch (readingType){
        case ReadAll:{
            qDebug()<<"read all "<<this->ReadingStarted;
            SHORT* adc;
            while(this->ReadingStarted)
            {


                if ((adc=reader->readData())!=0){
                    qDebug()<<adc[0];
                    buffer->append(adc[0],adc[1],adc[2]);
                    msleep(10);
                }
                else{
                    qDebug()<<"Get crashe";
                    msleep(10);
                }

            }
            delete adc;
            break;
        }
        case ReadForVolZer:{
            int avg=0;
            for(int i=0;i<300;i++)
            {
                SHORT* adc;
                if(ReadingStarted==false){
                    break;
                }
                if ((adc=reader->readData())!=0){
                    avg+=adc[0];
                    qDebug()<<"ReadForVolZer"<<adc[0];
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
        case ReadForVolVal:{
            buffer->setEnd(0);
            for(int i=0;i<1200;i++){
                if(ReadingStarted==false){
                    break;
                }
                SHORT* adc;
                if ((adc=reader->readData())!=0){
                    qDebug()<<"ReadForVolVal"<<adc[0];
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

    /*if(reader!=0){
        delete reader;
    }*/
    reader->closeReader();
    qDebug()<<"Before deeting reader";
   /* if(reader!=0){
        delete reader;
       // reader = 0;
    }*/
    qDebug()<<"A delete reader";
    emit done();
    qDebug()<<"WTF";
    ReadingStarted=true;
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
TSReaderThread::~TSReaderThread(){
    stopRead();
}
