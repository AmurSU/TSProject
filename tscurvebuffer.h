#ifndef TSCURVEBUFFER_H
#define TSCURVEBUFFER_H

#include <QObject>
#include <QColor>
#include <fstream>
#include <tsanalitics.h>
using namespace std;

#define VOLTAGE_RATE    0.61
#define REF_VOLTAGE_1   -4778
#define REF_VOLTAGE_2   -5615
#define TAN_1           0.0056
#define TAN_2           0.0048
#define REF_TEMP        -9

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
    void setVolumeColibration(int c);
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
    FILE* out_file;
    QColor volColor;
    QColor tinColor;
    QColor toutColor;
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
    //int ts_tempIn_extr
    int ts_max_lev[3], ts_min_lev[3];
    int ts_low_max_lev[3],ts_height_min_lev[3];
    int ts_avgTempIn, ts_avgTempOut;
    int ts_up_sqc_cnt, ts_up_sqc_max,ts_down_sqc_cnt, ts_down_sqc_min;
    int ts_up_TempIn_sum, ts_down_TempIn_sum, ts_up_TempOut_sum, ts_down_TempOut_sum;
    int ts_num_up_extr_TempIn;
    int ts_num_up_extr_TempOut;
    void findLevels();
    int ts_num_down_extr_TempIn;
    int ts_avgti,ts_avgto;
    int ts_vm_up_lvl, ts_vm_max, ts_vm_max_avg,ts_vm_up_cnt;
    int ts_sniff_cntr, ts_sniff_period_cntr,ts_avgDo;
    int ts_period_for_count_avgs;
    int ts_minTempIn;
    int ts_maxTempIn;
    int ts_minTempOut;
    int ts_maxTempOut;
    int ts_minVolume;
    int ts_maxVolume;
    ofstream volfile;
    int AvgTempIn,AvgTempOut,InspirationFrequency,BreathingVolume;
    tsanalitics *ga_it, *ga_ot, *ga_vo;
    int bv,bvo,dbv;
    QVector<int> BreathVolumes;

};

#endif // TSCURVEBUFFER_H
