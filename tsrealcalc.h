#ifndef TSREALCALC_H
#define TSREALCALC_H

#include <QObject>
#include "tstempanalitic.h"
#include "tsanalitics.h"
#include <QDebug>

class tsrealcalc : public QObject{
    Q_OBJECT
public:
    explicit tsrealcalc(QObject *parent = 0);
    ~tsrealcalc();
    void append(int in_temp,int out_temp, int volume);
public slots:
    void process();
signals:
    void finished();
    void updateAverageData(int avgTempIn, int avgTempOut, int avgDo, int ChD);
private:
    tstempanalitic ga_it, ga_ot;
    tsanalitics ga_vo;
};

#endif // TSREALCALC_H
