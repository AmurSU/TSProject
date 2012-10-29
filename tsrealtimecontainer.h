#ifndef TSREALTIMECONTAINER_H
#define TSREALTIMECONTAINER_H
#include "tsanalitics.h"
#include <QThread>

class tsrealtimecontainer : public QThread
{
    Q_OBJECT
public:
    explicit tsrealtimecontainer(QObject *parent = 0);
    void appendTi(int ti);
    void appendTo(int to);
    void appendVo(int vo);
    void calcParams();
private:
    tsanalitics *ga_it, *ga_ot, *ga_vo;
    int AvgTempIn,AvgTempOut,InspirationFrequency,BreathingVolume;
signals:
    void updateAverageData(int avgTempIn, int avgTempOut, int avgDo, int ChD);
public slots:
    
};

#endif // TSREALTIMECONTAINER_H
