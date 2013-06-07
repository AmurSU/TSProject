#ifndef TSREALCALCWRAPPER_H
#define TSREALCALCWRAPPER_H

#include <QObject>
#include <QVector>
#include <QThread>
#include "tsrealcalc.h"

class tsRealCalcWrapper : public QObject
{
    Q_OBJECT
public:
    explicit tsRealCalcWrapper(QObject *parent = 0);
    void addData(int tempIn, int tempOut, int volume);
    void reset();
    int getAvgTempIn();
    int getAvgTempOut();
    int getAvgInspirationVolume();
    int getInspirationFreqency();
public slots:
    void getAverageData(int avgTempIn, int avgTempOut, int avgDo, int inspFreq);
private:
    int AvgTempIn;
    int AvgTempOut;
    int AvgDo;
    int InspFreq;
    static const int CALC_PERIOD = 60;
    void calc();
    QVector<int> TempIn, TempOut, Volume;
};

#endif // TSREALCALCWRAPPER_H
