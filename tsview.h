#ifndef TSVIEW_H
#define TSVIEW_H

#include <QMainWindow>
#include "tscontroller.h"
#include "tsiview.h"
#include <QPen>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
    class TSView;
    class TSPatientProfile;
    class TSCalibrateDialog;
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
    void stopRecording();
    void showCalibrateionDialog(TSCalibrateDialogModel *model);
    void showModelMessage(QString mess);
    void closeModelMessage();
    void setController(TSController *c);
protected:
    void initPatientProfileUi();
private slots:
    void plotNow();
    void scrollValueChanged(int val);
    void f1();
private:
    Ui::TSView *ui;
    Ui::TSPatientProfile *patientProfileUi;
    TSController *control;
    Ui::TSCalibrateDialog *calibrateDialogUi;
    QDialog *calibrateDialog;
    QMessageBox message;
    QTimer timer1;
    // Все для рисования
    TSCurveBuffer* curveBuffer;
    QTimer plotingTimer;
    QPixmap bVolume;
    QPixmap bTempIn;
    QPixmap bTempOut;
    QPixmap bHorLine;
    QPainter pVolume;
    QPainter pTempIn;
    QPainter pTempOut;
    QPainter pHorLine;
    int* volume;
    int* tempIn;
    int* tempOut;
    int time;
    int screenLimit;
    int startIndex;
    int W;
    int H;
    bool recordingStarted;

};

#endif // TSVIEW_H
