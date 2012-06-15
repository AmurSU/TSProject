#ifndef TSVIEW_H
#define TSVIEW_H

#include <QMainWindow>
#include "tscontroller.h"
#include "tsiview.h"
#include <QPen>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

namespace Ui {
    class TSView;
    class TSPatientProfile;
}

class TSView : public QMainWindow,public TSIView
{
    Q_OBJECT
public:
    explicit TSView(QWidget *parent = 0);
    ~TSView();
    void showGUI();
    void showRecordWindow(TSCurveBuffer *curves);
    void setController(TSController *c);
    void showNewResearchDialog(TSPatientProfileModel *model);
    void startRealtimeViewing();
public slots:
    void drawCurves(CurvesSegnments s);
    void showCurves();
private:
    Ui::TSView *ui;
    Ui::TSPatientProfile *patientProfileUi;
    TSController *control;
    // Все для рисования
    TSCurveBuffer *cbuffer;
    QPixmap volPixmap;
    QPixmap tinPixmap;
    QPixmap toutPixmap;
    QPainter vol;
    QPainter tin;
    QPainter tout;
    QTimer *showCurvesTimer;
    QPen graphPen;
    int screenLimit;
    int h;
};

#endif // TSVIEW_H
