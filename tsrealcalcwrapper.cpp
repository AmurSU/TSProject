#include "tsrealcalcwrapper.h"

tsRealCalcWrapper::tsRealCalcWrapper(QObject *parent) :
    QObject(parent){
    AvgTempIn = 0;
    AvgTempOut = 0;
    AvgDo = 0;
    InspFreq = 0;
}

void tsRealCalcWrapper::addData(int tempIn = 0, int tempOut = 0, int volume = 0){
    TempIn.append(tempIn);
    TempOut.append(tempOut);
    Volume.append(volume);
    if ( TempIn.size()%CALC_PERIOD == 0 &&  TempIn.size()>0)
        calc();
   /* if ( TempIn.size()>500 ){
        TempIn.remove(0,1);
        TempOut.remove(0,1);
        Volume.remove(0,1);
    }*/
}

void tsRealCalcWrapper::reset(){
    AvgTempIn = 0;
    AvgTempOut = 0;
    AvgDo = 0;
    InspFreq = 0;
    TempIn.clear();
    TempOut.clear();
    Volume.clear();
}

int tsRealCalcWrapper::getAvgTempIn(){
    return AvgTempIn;
}

int tsRealCalcWrapper::getAvgTempOut(){
    return AvgTempOut;
}

int tsRealCalcWrapper::getAvgInspirationVolume(){
    return AvgDo;
}

int tsRealCalcWrapper::getInspirationFreqency(){
    return InspFreq;
}


void tsRealCalcWrapper::calc(){
    QThread* rcthread = new QThread();
    tsrealcalc* rcalc = new tsrealcalc();
    for (int i=0;i<TempIn.size();i++)
        rcalc->append(TempIn.at(i),TempOut.at(i),Volume.at(i));
    rcalc->moveToThread(rcthread);
    connect(rcthread,SIGNAL(started()),rcalc,SLOT(process()));
    connect(rcalc,SIGNAL(updateAverageData(int,int,int,int)),this,SLOT(getAverageData(int,int,int,int)));
    connect(rcalc,SIGNAL(finished()),rcthread,SLOT(quit()));
    connect(rcalc,SIGNAL(finished()),rcalc,SLOT(deleteLater()));
    connect(rcthread,SIGNAL(finished()),rcthread,SLOT(deleteLater()));
    rcthread->start();
}


void tsRealCalcWrapper::getAverageData(int avgTempIn, int avgTempOut, int avgDo, int inspFreq){
    AvgTempIn = avgTempIn;
    AvgTempOut = avgTempOut;
    AvgDo = avgDo;
    InspFreq = inspFreq;
}
