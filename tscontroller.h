#ifndef TSVIEW_H
#define TSVIEW_H

#include <QMainWindow>
#include <QRegExp>

namespace Ui {
    class TSView;
}

enum CurrentAction {NoAction,CreatePatientProfileAction,EditPatientProfileAction};

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
private:
    Ui::TSView *ui;
    CurrentAction currentAction;
    QRegExp nameRegExp;
    QRegExp intRegExp;
};

#endif // TSVIEW_H
