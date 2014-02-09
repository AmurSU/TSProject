#ifndef VOLUMESOLVER_H
#define VOLUMESOLVER_H
#include <QVector>
#include <QPoint>
#include <QDebug>
#include "math.h"
struct cycle{
    QPoint in,mid,ext,out;
};

class VolumeSolver
{
public:
    VolumeSolver(QVector<int> volume, QVector<int> tempin, QVector<int> tempout);
    QVector<QPoint> getSignal();
    float getAverageInspirationTime();
    float getAverageExpirationTime();
    float getMaxExpirationSpeed();
    float getAverageExpirationSpeed();
    float getInspirationFrequancyInOneMinute();
    float getAverageCycleTime();
    float getTotalVentilation();
    float getMinuteVentilation();
    float getAverageInspirationTempetature();
    float getAverageExpirationTempetature();
    float getAverageInspiratonVentilation();
private:
    void prepareData();
    QVector<QPoint> raw_data;
    QVector<QPoint> temp_in,temp_out;
    QVector<QPoint> prepared_data;
    QVector<cycle> cycles;
    void printSignal();
    void approximate();
    void deleteEqualNoisePoints();
    void takeCycles();
    int NOISE_DIFF = 10;
    int FREQUANCY = 100;
};

#endif // VOLUMESOLVER_H
