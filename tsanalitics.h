#ifndef TSANALITICS_H
#define TSANALITICS_H

#include <QObject>
#include <QVector>
#include <extremum.h>
struct exhalation {
    int start, end, vol;
};

class tsanalitics : public QObject
{
    Q_OBJECT
public:
    explicit tsanalitics(QObject *parent = 0);
    QVector<int> getMovingAverages(int Period);
    int getMaxAvgs();
    int getMinAvgs();
    float getFrequency(); //!!!volume
    float getAvgExpirationFlowRate();
    int getTime();
    int getAvgInspiratoryTime();
    int getAvgExpiratoryTime();
    int getPeriodTime();
    int setupData(QVector<int> *row_d);\
    void approximate();
    int getBreathingVolume();
    int getAvgExpiratorySpeed();
    int getMaxExpiratorySpeed();
    void append(int n);
    void clear();
    int getMaxsSum();
    int getMinsSum();
    int getMVL();
    int getMax();
    int getMin();
private:
    int fabs(int a);
    QVector<extremum> *ts_extremums;
    QVector<extremum> *ts_vol_exts;
    QVector<exhalation> *ts_exhls;
    QVector<int> *ts_row_data;
signals:
    
public slots:
    
};

#endif // TSANALITICS_H
