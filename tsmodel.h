#ifndef TSMODEL_H
#define TSMODEL_H

#include <QObject>
#include "tscurvebuffer.h"

class TSModel : public QObject
{
    Q_OBJECT
public:
    explicit TSModel(QObject *parent = 0);
    TSCurveBuffer* curveModel();

signals:

public slots:

private:
    TSCurveBuffer ts_curveModel;

};

#endif // TSMODEL_H
