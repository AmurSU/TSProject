#include "tsrealcalc.h"

tsrealcalc::tsrealcalc(QObject *parent) :QObject(parent){
}

tsrealcalc::~tsrealcalc(){
}

void tsrealcalc::append(int in_temp = 0, int out_temp = 0, int volume = 0 ){
    ga_it.append(in_temp);
    ga_vo.append(volume);
    ga_ot.append(out_temp);
}

void tsrealcalc::process(){
    int AvgTempIn,AvgTempOut,InspirationFrequency=0,BreathingVolume=0;
    ga_it.findExtremums();
    ga_it.deleteBadExtremums();
    AvgTempIn = ga_it.getMinAvgs();
    ga_it.clear();

    ga_ot.findExtremums();
    ga_ot.deleteBadExtremums();
    AvgTempOut = ga_ot.getMaxAvgs();
    ga_ot.clear();

    ga_vo.approximate();
    BreathingVolume = ga_vo.getBreathingVolume();
    InspirationFrequency = ga_vo.getFrequency();
    ga_vo.clear();

    // нада сделать чтобы дергалась функция в curvebuffer и вызывался сигнал обновления
    emit updateAverageData(AvgTempIn,AvgTempOut,BreathingVolume,InspirationFrequency);
    emit finished();
}
