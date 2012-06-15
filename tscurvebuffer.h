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
    void setValues(int* volume,int* tin,int* tout, int n);
    void append(int v, int tIn, int tO);
signals:
    void changed(CurvesSegnments s);
    void overflowed();
public slots:
private:
    int ts_volume[18000];
    int ts_tempIn[18000];
    int ts_tempOut[18000];
    int ts_end;
};

#endif // TSCURVEBUFFER_H
