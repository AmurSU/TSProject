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
    void showNewResearchDialog(TSPatientProfileModel *model);
    void showEditPatientProfileDialog(TSPatientProfileModel *model);
    void showResearchwindow(TSCurveBuffer *model);
    void startRecording();
    void setController(TSController *c);
protected:
    void initPatientProfileUi();
private slots:
    void plotNow();
private:
    Ui::TSView *ui;
    Ui::TSPatientProfile *patientProfileUi;
    TSController *control;
    // ��� ��� ���������
    TSCurveBuffer* curveBuffer;
    QTimer plotingTimer;
    QPixmap bVolume;
    QPixmap bTempIn;
    QPixmap bTempOut;
    QPainter pVolume;
    QPainter pTempIn;
    QPainter pTempOut;
    int* volume;
    int* tempIn;
    int* tempOut;
    int screenLimit;
    int startIndex;
    int W;
    int H;
};

#endif // TSVIEW_H
