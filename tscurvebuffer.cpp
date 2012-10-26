#include "tscurvebuffer.h"
#include <QDebug>
#include <tsanalitics.h>

TSCurveBuffer::TSCurveBuffer(QObject *parent) :
    QObject(parent)
{
    ts_end = -1;
    ts_monoInterval_in[0].end=0;
    ts_monoInterval_in[0].start=0;
    ts_monoInterval_in[0].znak=0;
    ts_monoInterval_out[0].end=0;
    ts_monoInterval_out[0].start=0;
    ts_monoInterval_out[0].znak=0;
    ts_volumeColibration = 0;
    ts_volumePosConvert=1;
    ts_volumeNegConvert=-1;
    out_file=fopen("output.txt","w");
    int i;
    max_v=-1100000;
    min_v=10000;
    /*for(i=0;i<3;i++){
        ts_max_lev[i]=-100000;
        ts_min_lev[i]=1000000;
    }
    ts_up_sqc_cnt=0;
    ts_up_sqc_max=-1000000;
    ts_down_sqc_cnt=0;
    ts_down_sqc_min=1000000;

    ts_up_TempIn_sum=0;
    ts_down_TempIn_sum=0;
    ts_up_TempOut_sum=0;
    ts_down_TempOut_sum=0;

    ts_vm_up_lvl=ts_volumeColibration+2;
    ts_vm_max=-100000;
    ts_period_for_count_avgs=50;*/
    volfile.open("volume.csv");
    ga_it = new tsanalitics();
    ga_ot = new tsanalitics();
    ga_vo = new tsanalitics();
}

int TSCurveBuffer::end()
{
    return ts_end;
}

int* TSCurveBuffer::volume()
{
    return ts_integral;
}

int* TSCurveBuffer::tempIn()
{
    return ts_tempIn;
}

int* TSCurveBuffer::tempOut()
{
    return ts_tempOut;
}

void TSCurveBuffer::append(int v, int tI, int tO, bool realtime)
{
    if(ts_end == 17999)
    {
        emit overflowed();
        return;
    }

    if(realtime)
    {
        v*= VOLTAGE_RATE;
        tI*= VOLTAGE_RATE;
        tO*= VOLTAGE_RATE;
    }

    if(ts_end == 0)
    {
        ts_minTempIn = tI;
        ts_maxTempIn = tI;
        ts_minTempOut = tO;
        ts_maxTempOut = tO;
    }
    else
    {
        if(tI<=ts_minTempIn) ts_minTempIn=tI;
        if(tI>ts_maxTempIn) ts_maxTempIn=tI;
        if(tO<=ts_minTempOut) ts_minTempOut=tO;
        if(tO>ts_maxTempOut) ts_maxTempOut=tO;
    }
    ga_it->append(ts_tempIn[ts_end-1]);
    ga_ot->append(ts_tempOut[ts_end-1]);
    ga_vo->append(ts_integral[ts_end-1]);
    if(realtime){
        int num=500;
        if(ts_end%num==0){
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
    }
    v -= ts_volumeColibration;
    CurvesSegnments segs;
    if(ts_end>0)
    {
        if (ts_integral[ts_end-1]>=max_v){
            max_v=ts_integral[ts_end-1];
            maxc_v=ts_end-1;
        }
        if (ts_integral[ts_end-1]<=min_v){
            min_v=ts_integral[ts_end-1];
            minc_v=ts_end-1;
        }
        segs.prevV = ts_volume[ts_end];
        segs.prevTin = ts_tempIn[ts_end];
        segs.prevTout = ts_tempOut[ts_end];
        if(abs(v)>=8)
        {
            if(realtime){
                ts_integral[ts_end] = 0.1*v + ts_integral[ts_end-1];
                if( ts_end-maxc_v>1 && v>10){
                    ts_integral[ts_end]=0;
                    qDebug()<<"Oh shit, we should obrezat this verhushka.";
                    max_v=-10000;
                    maxc_v=ts_end;
                }else
                    if( ts_end-minc_v>1 && v<-10){
                        ts_integral[ts_end]=0;
                        qDebug()<<"Oh shit, we should obrezat this nizushka.";
                        min_v=10000;
                        minc_v=ts_end;
                    }
            }
            else
                ts_integral[ts_end] = v;
        }
        else ts_integral[ts_end]=0;
        if(ts_integral[ts_end]>ts_maxVolume)ts_maxVolume=ts_integral[ts_end];
        if(ts_integral[ts_end]<ts_minVolume)ts_minVolume=ts_integral[ts_end];
    }
    else
    {
        ts_integral[0] = 0;
        ts_minVolume = ts_integral[0];
        ts_minVolume = ts_integral[0];
    }
    ts_volume[ts_end] = segs.curV = v;
    ts_tempIn[ts_end] = segs.curTin = tI;
    ts_tempOut[ts_end] = segs.curTout = tO;
    emit changed(segs);
    ts_end++;
}
/*
void TSCurveBuffer::findLevels(){
    if(ts_end%ts_period_for_count_avgs+1){
        ts_low_max_lev[0]=0;
        ts_low_max_lev[1]=0;
        ts_low_max_lev[2]=0;
        ts_height_min_lev[0]=0;
        ts_height_min_lev[1]=0;
        ts_height_min_lev[2]=0;
        ts_max_lev[0]=-1000000;
        ts_max_lev[1]=-1000000;
        ts_max_lev[2]=-1000000;
        ts_min_lev[0]=1000000;
        ts_min_lev[1]=1000000;
        ts_min_lev[2]=1000000;
    }
    if(ts_end>40){
        if(ts_volume[ts_end]>ts_max_lev[0])
            ts_max_lev[0]=ts_volume[ts_end];
        if(ts_tempIn[ts_end]>ts_max_lev[1])
            ts_max_lev[1]=ts_tempIn[ts_end];
        if(ts_tempOut[ts_end]>ts_max_lev[2])
            ts_max_lev[2]=ts_tempOut[ts_end];

        if(ts_volume[ts_end]<ts_min_lev[0])
            ts_min_lev[0]=ts_volume[ts_end];
        if(ts_tempIn[ts_end]<ts_min_lev[1])
            ts_min_lev[1]=ts_tempIn[ts_end];
        if(ts_tempOut[ts_end]<ts_min_lev[2])
            ts_min_lev[2]=ts_tempOut[ts_end];
        if (ts_end%40==0){
            ts_low_max_lev[0]=ts_max_lev[0]-(ts_max_lev[0]-ts_min_lev[0])*0.3;
            ts_low_max_lev[1]=ts_max_lev[1]-(ts_max_lev[1]-ts_min_lev[1])*0.3;
            ts_low_max_lev[2]=ts_max_lev[2]-(ts_max_lev[2]-ts_min_lev[2])*0.3;

            ts_height_min_lev[0]=ts_min_lev[0]+(ts_max_lev[0]-ts_min_lev[0])*0.3;
            ts_height_min_lev[1]=ts_min_lev[0]+(ts_max_lev[1]-ts_min_lev[1])*0.3;
            ts_height_min_lev[2]=ts_min_lev[0]+(ts_max_lev[2]-ts_min_lev[2])*0.3;
            qDebug()<<ts_low_max_lev[0]<<ts_height_min_lev[0]<<ts_low_max_lev[1]<<ts_height_min_lev[1]<<ts_low_max_lev[2]<<ts_height_min_lev[2];
        }
    }
}*/

void TSCurveBuffer::setValues(int *vol, int *tin, int *tout, int n)
{
    //if(ts_end != -1)return;
    for(int i=0;i<n;i++)
    {
        append(vol[i],tin[i],tout[i],false);
    }
}

void TSCurveBuffer::setVolumeColibration(int c,bool realtime = true)
{
    if(realtime)
        ts_volumeColibration = c * VOLTAGE_RATE;
    else
        ts_volumeColibration = c;
}


void TSCurveBuffer::setScreenLimit(int sl)
{
    ts_screenLimit =sl;
}

int TSCurveBuffer::startIndex()
{
    return ts_startIndex;
}

void TSCurveBuffer::setStartIndex(int s)
{
    ts_startIndex = s;
}

void TSCurveBuffer::setEnd(int n)
{
    ts_end = n;
}
int TSCurveBuffer::screenLimit()
{
    return ts_screenLimit;
}


int* TSCurveBuffer::getTempInInterval()
{
    int *interval = new int[2];
    if(abs(ts_maxTempIn-ts_minTempIn)<500)
    {
        interval[0]=-5000;
        interval[1]=5000;
    }
    else
    {
        interval[0]=ts_minTempIn-100;
        interval[1]=ts_maxTempIn+100;
    }
    return interval;
}

int* TSCurveBuffer::getTempOutInterval()
{
    int *interval = new int[2];
    if(abs(ts_maxTempOut-ts_minTempOut)<500)
    {
        interval[0]=-5000;
        interval[1]=5000;
    }
    else
    {
        interval[0]=ts_minTempOut-100;
        interval[1]=ts_maxTempOut+100;
    }
    return interval;
}

int* TSCurveBuffer::getVolumeInterval()
{
    int *interval = new int[2];
    if(abs(ts_maxVolume-ts_minVolume)<250)
    {
        interval[0]=-5000;
        interval[1]=5000;
    }
    else
    {
        if(abs(ts_minVolume)>ts_maxVolume)
        {
            interval[0]=ts_minVolume-120;
            interval[1]=(-ts_minVolume)+100;
        }
        else
        {
            interval[0]=-ts_maxVolume-120;
            interval[1]=ts_maxVolume+100;
        }
    }
    return interval;
}


int TSCurveBuffer::volumeColibration()
{
    return ts_volumeColibration;
}


void TSCurveBuffer::setVolumeConverts(int pos, int neg)
{
    ts_volumePosConvert=pos;
    ts_volumeNegConvert=neg;
}

QVector<int> TSCurveBuffer::volumeConverts(){
    QVector<int> v;
    v.push_back(ts_volumeNegConvert);
    v.push_back(ts_volumePosConvert);
    return v;
}

float TSCurveBuffer::volToLtr(int vol)
{
    if(vol<0)
    {
        return (float)vol/ts_volumeNegConvert;
    }
    else
    {
        return (float)vol/ts_volumePosConvert;
    }
}

float TSCurveBuffer::tempInToDeg(int temp)
{
    return TAN_1*(REF_VOLTAGE_1-temp);//+(float)REF_TEMP;
}

float TSCurveBuffer::tempOutToDeg(int temp)
{
    return TAN_2*(REF_VOLTAGE_2-temp);//+(float)REF_TEMP;
}

void TSCurveBuffer::clean(){
    ts_end=0;
    ga_it->clear();
    ga_ot->clear();
    ga_vo->clear();
}

int TSCurveBuffer::setReference(QSettings *set)
{
    if(!set->contains("RefTemp")||!set->contains("tempInVolt")||
            !set->contains("tanTempIn")||!set->contains("tempOutVolt")||
            !set->contains("tanTempOut")){
        qDebug()<<"settings.ini error";
        return 1;
    }
    else{
        ts_refTemp = set->value("RefTemp").toInt();
        ts_tempInVolt = set->value("tempInVolt").toInt();
        ts_tempOutVolt = set->value("tempOutVolt").toInt();
        ts_tanTempIn = set->value("tanTempIn").toDouble();
        ts_tanTempOut = set->value("tanTempOut").toDouble();
    }
}
