#ifndef TSCONTROLLER_H
#define TSCONTROLLER_H

#include <QObject>
#include "tsiview.h"
#include "tsmodel.h"
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
private:
    TSIView *view;
    TSModel *model;
    QTimer* requestTimer;
};

#endif // TSCONTROLLER_H
