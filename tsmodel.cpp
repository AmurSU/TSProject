#include "tsmodel.h"

TSModel::TSModel(QObject *parent) :
    QObject(parent)
{
}


TSCurveBuffer* TSModel::curveModel()
{
    return &ts_curveModel;
}
