#ifndef TSCURVEBUFFER_H
#define TSCURVEBUFFER_H

#include <QObject>

struct CurvesSegnments
{
    int curV,prevV;
    int curTin,prevTin;
    int curTout,prevTout;
};

class TSCurveBuffer : public QObject
{
    Q_OBJECT
public:
    explicit TSCurveBuffer(QObject *parent = 0);
    int end();
    CurvesSegnments lastSegments();
    int* tempIn();
    int* tempOut();
    int* volume();
    void setVolumeColibration(int c);
    void setValues(int* volume,int* tin,int* tout, int n);
    void append(int v, int tIn, int tO);
    void setScreenLimit(int sl);
    int startIndex();
    void setStartIndex(int s);
    void setEnd(int e);
signals:
    void changed(CurvesSegnments s);
    void overflowed();
public slots:
private:
    int ts_volume[18000];
    int ts_tempIn[18000];
    int ts_tempOut[18000];
    int ts_integral[18000];
    int ts_end;
    int ts_volumeColibration;
    int ts_screenLimit;
    int ts_startIndex;
};

#endif // TSCURVEBUFFER_H
