#include "tscurvebuffer.h"
#include <QDebug>

TSCurveBuffer::TSCurveBuffer(QObject *parent) :
    QObject(parent)
{
    ts_end = -1;
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

void TSCurveBuffer::append(int v, int tI, int tO)
{
    if(ts_end == 17999)
    {
        emit overflowed();
        return;
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
    if(ts_end+35>=ts_screenLimit)
    {
        ts_startIndex+=10;
        ts_screenLimit+=10;
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
    //qDebug()<<ts_volume[ts_end];
    ts_tempIn[ts_end] = segs.curTin = tI;
    ts_tempOut[ts_end] = segs.curTout = tO;
    emit changed(segs);
}

void TSCurveBuffer::setValues(int *vol, int *tin, int *tout, int n)
{
    if(ts_end != 0)return;
    for(int i=0;i<n;i++)
    {
        append(vol[i],tin[i],tout[i]);
    }
}

void TSCurveBuffer::setVolumeColibration(int c)
{
    ts_volumeColibration =c;
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
