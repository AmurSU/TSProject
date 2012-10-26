#ifndef TSCURVEBUFFER_H
#define TSCURVEBUFFER_H

#include <QObject>
#include <QColor>
#include <fstream>
#include <QSettings>
#include <tsanalitics.h>
#include <QVector>
#include <tsanalitics.h>
using namespace std;

#define VOLTAGE_RATE    0.61
#define REF_VOLTAGE_1   ts_tempInVolt
#define REF_VOLTAGE_2   ts_tempOutVolt
#define TAN_1           ts_tanTempIn
#define TAN_2           ts_tanTempOut
#define REF_TEMP        ts_refTemp

struct CurvesSegnments
{
    int curV,prevV;
    int curTin,prevTin;
    int curTout,prevTout;
};
struct MonotonicityInterval
{
    int start,end,znak;
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
    void setVolumeColibration(int c,bool realtime);
    void setValues(int* volume,int* tin,int* tout, int n);
    void append(int v, int tI, int tO, bool realtime = true);
    void setScreenLimit(int sl);
    int startIndex();
    void setStartIndex(int s);
    void setEnd(int n);
    int screenLimit();
    int* getTempInInterval();
    int* getTempOutInterval();
    int* getVolumeInterval();
    int volumeColibration();
    void setVolumeConverts(int pos,int neg);
    QVector<int> volumeConverts();
    float volToLtr(int vol);
    int setReference(QSettings *set);
    float tempInToDeg(int temp);
    float tempOutToDeg(int temp);
    FILE* out_file;
    QColor volColor;
    QColor tinColor;
    QColor toutColor;
    void clean();
signals:
    void changed(CurvesSegnments s);
    void overflowed();
    void updateAverageData(int avgTempIn, int avgTempOut, int avgDo, int ChD);
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
    MonotonicityInterval ts_monoInterval_in[1000];
    int ts_mi_in_cnt;
    MonotonicityInterval ts_monoInterval_out[1000];
    int ts_mi_out_cnt;
    int ts_check;
    void findLevels();
    int max_v,maxc_v,min_v,minc_v;

    int ts_minTempIn;
    int ts_maxTempIn;
    int ts_minTempOut;
    int ts_maxTempOut;
    int ts_minVolume;
    int ts_maxVolume;
    int ts_volumePosConvert;
    int ts_volumeNegConvert;
    float ts_tanTempIn;
    float ts_tanTempOut;
    int ts_tempInVolt;
    int ts_tempOutVolt;
    int ts_refTemp;
    ofstream volfile;
    int AvgTempIn,AvgTempOut,InspirationFrequency,BreathingVolume;
    tsanalitics *ga_it, *ga_ot, *ga_vo;
    int bv,bvo,dbv;
    QVector<int> BreathVolumes;

};

#endif // TSCURVEBUFFER_H
