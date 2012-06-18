#include "tscurvebuffer.h"

TSCurveBuffer::TSCurveBuffer(QObject *parent) :
    QObject(parent)
{
    ts_volume[0]=0;
    ts_tempIn[0]=0;
    ts_tempOut[0]=0;
    ts_end = 0;
}

int TSCurveBuffer::end()
{
    return ts_end;
}

int* TSCurveBuffer::volume()
{
    return ts_volume;
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
    CurvesSegnments segs;
    segs.prevV = ts_volume[ts_end];
    segs.prevTin = ts_tempIn[ts_end];
    segs.prevTout = ts_tempOut[ts_end];
    ts_end++;
    ts_volume[ts_end] = segs.curV = v;
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
