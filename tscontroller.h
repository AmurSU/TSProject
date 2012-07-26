#ifndef TSCONTROLLER_H
#define TSCONTROLLER_H

#include <QObject>
#include "tsiview.h"
#include "tsmodel.h"
#include "tsreaderthread.h"
#include <QTimer>

class TSController : public QObject
{
    Q_OBJECT
public:
    explicit TSController(TSIView *iview, TSModel *mod,QObject *parent = 0);
    void handle();
signals:

public slots:
    void newResearchRequesdted();
    void editPatientProfileRequested();
    void newResearchAccepted();
    void startRecordingRequested();
    void stopRecoringRequested();
    void calibrateDialogRequested();
    void calibrateDialogAccepted();
    void startVolumeCalibrationRequested();

private:
    TSIView *view;
    TSModel *model;
    QTimer* requestTimer;
    TSReaderThread  *trd;
};

#endif // TSCONTROLLER_H
