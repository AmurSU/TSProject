#include "tsrealcalc.h"

tsrealcalc::tsrealcalc(QObject *parent) :QObject(parent){
}

tsrealcalc::~tsrealcalc(){
}

void tsrealcalc::append(int in_temp = 0, int out_temp = 0, int volume = 0 ){
    vo.push_back(volume);
    ti.push_back(in_temp);
    to.push_back(out_temp);
}

void tsrealcalc::process(){
    VolumeSolver vs(vo,ti,to);
    float AvgTempIn,AvgTempOut,InspirationFrequency=0,BreathingVolume=0;
    AvgTempIn = vs.getAverageInspirationTempetature();
    AvgTempOut = vs.getAverageExpirationTempetature();
    InspirationFrequency = vs.getInspirationFrequancyInOneMinute();
    BreathingVolume = vs.getAverageInspiratonVentilation();
    // нада сделать чтобы дергалась функция в curvebuffer и вызывался сигнал обновления
    emit updateAverageData((int)AvgTempIn,(int)AvgTempOut,(int)BreathingVolume,(int)InspirationFrequency);
    emit finished();
}
