#ifndef TSIVIEW_H
#define TSIVIEW_H

#include "ts_types.h"

class TSIView
{
public:
    virtual void showGUI()=0;
    virtual void showNewResearchDialog(TSPatientProfileModel *model)=0;
    virtual void showEditPatientProfileDialog(TSPatientProfileModel *model)=0;
    virtual void showResearchwindow(TSCurveBuffer *model)=0;
    virtual void startRecording()=0;
    virtual void stopRecording()=0;
    virtual void showCalibrateionDialog(TSCalibrateDialogModel *model)=0;
    virtual void showModelMessage(QString mess)=0;
    virtual void closeModelMessage()=0;
};

#endif // TSIVIEW_H
