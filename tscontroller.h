#ifndef TSVIEW_H
#define TSVIEW_H

#include <QMainWindow>
#include <QMessageBox>
#include <tscurvebuffer.h>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <tsreaderthread.h>
#include <tsusb3000reader.h>
#include <QRegExp>

namespace Ui {
    class TSView;
}

enum CurrentAction {NoAction,CreatePatientProfileAction,EditPatientProfileAction,GetingVolZero};

class TSController : public QMainWindow
{
    Q_OBJECT

public:
    explicit TSController(QWidget *parent = 0);
    ~TSController();

public slots:
    void incCurrentIndex();
    void decCurrentIndex();
    void editPatientProfile();
    void savePatientProfile();
    void rejectPatientProfile();
    void calibrateVolume();
    void calibrateTemperature();
    void rejectColibration();
    void threadFinished();
    void startExam();
    void plotNow();
protected:
    void initPaintDevices();
private:
    Ui::TSView *ui;
    QMessageBox *msgBox;
    CurrentAction currentAction;
    QRegExp nameRegExp;
    QRegExp intRegExp;
    //Все для рисования
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
    bool recordingStarted;
    //Тред для чтения
    TSReaderThread *readerThread;

};

#endif // TSVIEW_H
