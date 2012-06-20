#ifndef TS_TYPES_H
#define TS_TYPES_H

#include <QStringList>
#include "tscurvebuffer.h"

struct TSPatientProfileModel
{
    QString sname,name,iname,age,gender,mvl,id;
};

struct TSCalibrateDialogModel
{
    int colibrateVolume;
};

#endif // TS_TYPES_H
