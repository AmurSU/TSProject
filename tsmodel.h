#ifndef TSMODEL_H
#define TSMODEL_H

#include <QObject>
#include "tscurvebuffer.h"
#include "ts_types.h"

class TSModel : public QObject
{
    Q_OBJECT
public:
    explicit TSModel(QObject *parent = 0);
    TSCurveBuffer* curveModel();
    TSPatientProfileModel* patientProfileModel();

signals:

public slots:

private:
    TSCurveBuffer ts_curveModel;
    TSPatientProfileModel ts_patientProfileModel;

};

#endif // TSMODEL_H
