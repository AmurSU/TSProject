#ifndef TSIVIEW_H
#define TSIVIEW_H

#include "tscurvebuffer.h"
#include "ts_types.h"

class TSIView
{
public:
    virtual void showGUI()=0;
    virtual void showRecordWindow(TSCurveBuffer *curves)=0;
    virtual void showNewResearchDialog(TSPatientProfileModel *model)=0;
    virtual void startRealtimeViewing()=0;
};

#endif // TSIVIEW_H
