#include "tscurvebuffer.h"
#include <QDebug>

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
    out_file=fopen("output.txt","w");
    int i;
    for(i=0;i<3;i++){
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
    ts_period_for_count_avgs=500;
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

    float tt,ttt;
    tt = TAN_1*(tI-REF_VOLTAGE_1)+REF_TEMP;
    ttt = TAN_2*(tO-REF_VOLTAGE_1)+REF_TEMP;

    //qDebug()<<tt<<" "<<ttt;

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
    }
    else
    {
        if(tI<ts_minTempIn) ts_minTempIn=tI;
        if(tI>ts_maxTempIn) ts_maxTempIn=tI;
    }

    findLevels();
    const int time_to_start_count_avgs=510, period_for_count_avgs=500;
    //qDebug()<<ts_volume[ts_end];
    if( ts_end > time_to_start_count_avgs ){
        //Count maxinmums of tempOut
        if(ts_tempOut[ts_end]>ts_low_max_lev[2]){
            if (ts_tempOut[ts_end]>ts_up_sqc_max)
                ts_up_sqc_max=ts_tempOut[ts_end];
            ts_up_sqc_cnt++;
        }
        if(ts_up_sqc_cnt>0 && ts_tempOut[ts_end]<ts_height_min_lev[2]){
            ts_num_up_extr_TempOut++;
            ts_up_TempOut_sum+=ts_up_sqc_max;
            ts_up_sqc_cnt=0;
            ts_up_sqc_max=-100000;
        }

        //Count minimums of tempIn
        if(ts_tempIn[ts_end]<ts_height_min_lev[1]){
            if (ts_tempIn[ts_end]<ts_down_sqc_min)
                ts_down_sqc_min=ts_tempIn[ts_end];
            ts_down_sqc_cnt++;
        }
        if(ts_down_sqc_cnt>0 && ts_tempIn[ts_end]>ts_height_min_lev[1]){
            ts_num_down_extr_TempIn++;
            ts_down_TempIn_sum+=ts_down_sqc_min;
            ts_down_sqc_cnt=0;
            ts_down_sqc_min=100000;
        }
        //find volume paraneters

        //ts_vm_up_lvl
        if( ts_integral[ts_end] > ts_low_max_lev[0] && ts_vm_up_cnt>=0){
            if( ts_integral[ts_end]>ts_vm_max ){
                ts_vm_max=ts_integral[ts_end];
            }
            ts_vm_up_cnt++;
        }
        if( ts_volume[ts_end] < ts_low_max_lev[0] && ts_vm_up_cnt>0){
            ts_vm_up_cnt=0;
            ts_vm_max_avg+=ts_vm_max;
            ts_sniff_period_cntr++;
            ts_sniff_cntr++;
            ts_vm_max=-100000;
        }

        if(ts_end%ts_period_for_count_avgs==0 ){

            if( ts_num_up_extr_TempOut>0 && ts_num_down_extr_TempIn>0  && ts_sniff_period_cntr>0){
                ts_avgto=ts_up_TempOut_sum/ts_num_up_extr_TempOut;
                ts_avgti=ts_down_TempIn_sum/ts_num_down_extr_TempIn;
                ts_avgDo=ts_vm_max_avg/ts_num_up_extr_TempOut;
                emit updateAverageData(ts_avgti,ts_avgto,ts_avgDo,ts_num_up_extr_TempOut*12);
                qDebug()<<"avgDO"<<ts_avgDo<<"ts_sniff_period_cntr"<<ts_sniff_period_cntr<<"ts_low_max_lev[0]="<<ts_low_max_lev[0];
                qDebug()<<"avgto="<<ts_avgto<<"; ts_num_up_extr_TempOut="<<ts_num_up_extr_TempOut;
                qDebug()<<"avgti="<<ts_avgti<<"; ts_num_up_extr_TempIn="<<ts_num_down_extr_TempIn;
            }
            ts_avgti=0;
            ts_down_TempIn_sum=0;
            ts_num_down_extr_TempIn=0;
            ts_avgto=0;
            ts_up_TempOut_sum=0;
            ts_num_up_extr_TempOut=0;
            ts_avgDo=0;
            ts_vm_max_avg=0;
            ts_sniff_period_cntr=0;
        }
    }


    v -= ts_volumeColibration;
    CurvesSegnments segs;
    if(ts_end>0)
    {
        segs.prevV = ts_volume[ts_end];
        segs.prevTin = ts_tempIn[ts_end];
        segs.prevTout = ts_tempOut[ts_end];
    }
    else
    {
        ts_integral[0] = 0;
    }
    ts_end++;
    if(ts_end>ts_screenLimit)
    {
        int dif = ts_end - ts_screenLimit;
        //ts_startIndex += dif;
        //ts_screenLimit += dif;
    }
    if(ts_end>0&&abs(v)>=8)
    {
        ts_integral[ts_end] = 0.01*v + ts_integral[ts_end-1];
    }
    else
    {
        ts_integral[ts_end] = 0;
    }
    ts_volume[ts_end] = segs.curV = v;
    ts_tempIn[ts_end] = segs.curTin = tI;
    ts_tempOut[ts_end] = segs.curTout = tO;
    emit changed(segs);
}

void TSCurveBuffer::findLevels(){
    if(ts_end%ts_period_for_count_avgs){
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
    if(ts_end>100){
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
        if (ts_end%100==0){
            ts_low_max_lev[0]=ts_max_lev[0]-(ts_max_lev[0]-ts_min_lev[0])*0.3;
            ts_low_max_lev[1]=ts_max_lev[1]-(ts_max_lev[1]-ts_min_lev[1])*0.3;
            ts_low_max_lev[2]=ts_max_lev[2]-(ts_max_lev[2]-ts_min_lev[2])*0.3;

            ts_height_min_lev[0]=ts_min_lev[0]+(ts_max_lev[0]-ts_min_lev[0])*0.3;
            ts_height_min_lev[1]=ts_min_lev[0]+(ts_max_lev[1]-ts_min_lev[1])*0.3;
            ts_height_min_lev[2]=ts_min_lev[0]+(ts_max_lev[2]-ts_min_lev[2])*0.3;
            fprintf(out_file,"%d; %d; %d; %d; %d; %d;\n",ts_low_max_lev[0],ts_height_min_lev[0],
                    ts_low_max_lev[1],ts_height_min_lev[1],ts_low_max_lev[2],ts_height_min_lev[2]);
        }
    }
}

void TSCurveBuffer::setValues(int *vol, int *tin, int *tout, int n)
{
    if(ts_end != -1)return;
    for(int i=0;i<n;i++)
    {
        append(vol[i],tin[i],tout[i],false);
    }
}

void TSCurveBuffer::setVolumeColibration(int c)
{
    ts_volumeColibration = c * VOLTAGE_RATE;
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
        interval[0]=ts_minTempIn-50;
        interval[1]=ts_maxTempIn+50;
    }
    return interval;
}
