#include "tsmodel.h"

TSModel::TSModel(QObject *parent) :
    QObject(parent)
{
}


TSCurveBuffer* TSModel::curveModel()
{
    return &ts_curveModel;
}

TSPatientProfileModel* TSModel::newResearchModel()
{
    return &ts_patientProfileModel;
}

TSCalibrateDialogModel* TSModel::calibrateModel()
{
    return &ts_calibrateModel;
}
