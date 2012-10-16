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
#include "models/TSExaminations.h"
#include "models/TSPatients.h"
#include <QTableWidget>
namespace Ui {
    class TSView;
    class TSProgressDialog;
    class MainWindow;
    class TSVolSignalWidget;
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
    void stopExam();
    void scrollGraphics(int value);
    void plotNow();
    void openPatientList();
    void completePatientName(QString string);
    void openPatientProfile(QModelIndex ind);
    void showAverageData(int avgTempIn, int avgTempOut,int avgDO, int avgCHD);
    void completePatientId();
    void createNewExam();
    void openExam(QModelIndex ind);
    void scaleTempIn(int value);
    void scaleTempOut(int value);
    void scaleVolume(int value);
    void scaleForHorizontal(int value);
    void changeScrollBarAfterScaling(int before,int after);
    void changeTempInScrollValue(int value);
    void changeTempOutScrollValue(int value);
    void breakExam();
    void processDataParams();
protected:
    void initPaintDevices();
    void resizeEvent(QResizeEvent *evt);
    bool eventFilter(QObject *obj, QEvent *e);
    void openPrivateDB(QSqlRecord record);
private:
    QTableWidgetItem* getQTableWidgetItem(QVariant text);
    Ui::TSView *ui;
    Ui::MainWindow *w;
    Ui::TSVolSignalWidget *volWidget;
    CurrentAction currentAction;
    QRegExp nameRegExp;
    QRegExp intRegExp;
    bool openUser;
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
    float volumeScaleRate;
    float tempInScaleRate;
    float tempOutScaleRate;
    float horizontalStep;
    int tempInZerPos;
    int tempOutZerPos;
    int scaleScroll[5];
    int* tempInInterval;
    float tempInAdaptive;
    int* tempOutInterval;
    float tempOutAdaptive;
    float volumeAdaptive;
    //Тред для чтения
    TSReaderThread *readerThread;
    //модели
    TSPatients *patientsModel;
    TSExaminations *examinationsModel;
    QSqlDatabase patientsConnection;
    QSqlDatabase examinationsConnection;
};

#endif // TSVIEW_H
