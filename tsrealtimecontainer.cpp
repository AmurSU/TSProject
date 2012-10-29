#include "tsrealtimecontainer.h"

tsrealtimecontainer::tsrealtimecontainer(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);
    ga_it = new tsanalitics();
    ga_ot = new tsanalitics();
    ga_vo = new tsanalitics();
}

void tsrealtimecontainer::appendTi(int ti)
{
    ga_it->append(ti);
}

void tsrealtimecontainer::appendTo(int to)
{
    ga_ot->append(to);
}

void tsrealtimecontainer::appendVo(int vo)
{
    ga_vo->append(vo);
}

void tsrealtimecontainer::calcParams()
{
    int num=500;

    int sum=0,i=0;
    ga_it->findExtremums();
    ga_it->deleteBadExtremums();
    AvgTempIn = ga_it->getMinAvgs();
    ga_it->clear();
    //qDebug()<<"AvgTempIn="<<AvgTempIn;

    ga_ot->findExtremums();
    ga_ot->deleteBadExtremums();
    AvgTempOut = ga_ot->getMaxAvgs();
    ga_ot->clear();
    //qDebug()<<"AvgTempOut="<<AvgTempOut;

    ga_vo->findExtremums();
    ga_vo->deleteBadExtremums();
    BreathingVolume = ga_vo->getBreathingVolume();
    /*BreathVolumes.append(BreathingVolume);

        for(i=0;i<BreathVolumes.size();i++){
            sum+=BreathVolumes.at(i);
        }
        BreathingVolume = (sum+BreathingVolume)/(i+1);*/

    InspirationFrequency = ga_vo->getFrequency();
    //qDebug()<<"BreathingVolume"<<BreathingVolume;
    //qDebug()<<"InspirationFrequency"<<InspirationFrequency;

    ga_vo->clear();
    emit updateAverageData(AvgTempIn,AvgTempOut,BreathingVolume,InspirationFrequency);
}
