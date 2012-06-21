#include "tsview.h"
#include "ui_tsview.h"
#include "ui_patientprofile.h"
#include "ui_colibrateDialog.h"
#include <QDebug>
#include <QDialog>
#include <QStringList>

TSView::TSView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TSView)
{
    ui->setupUi(this);
    patientProfileUi = 0;
    calibrateDialogUi = 0;
    ui->cbStart->setVisible(false);
    ui->cbStop->setVisible(false);
    ui->timeLabel->setVisible(false);
    recordingStarted = false;
    time=0;
}

TSView::~TSView()
{
    delete ui;
}

void TSView::showGUI()
{
    showMaximized();
}

void TSView::setController(TSController *c)
{
    control = c;
    connect(ui->actionNewResearch,SIGNAL(triggered()),control,SLOT(newResearchRequesdted()));
    connect(ui->actionPatient,SIGNAL(triggered()),control,SLOT(editPatientProfileRequested()));
    connect(ui->actionColibrate,SIGNAL(triggered()),control,SLOT(calibrateDialogRequested()));
}

void TSView::initPatientProfileUi()
{
    if(patientProfileUi != 0) return;
    else
    {
        patientProfileUi = new Ui::TSPatientProfile;
    }
}

void TSView::plotNow()
{
    int endIndex = curveBuffer->end();
    int h = ui->gVolume->height()/2;
    int h1 = ui->gVolume->height()-5;
    if(endIndex >= 17999)
    {
        plotingTimer.stop();
        return;
    }
    startIndex = curveBuffer->startIndex();
    int step = h/10;
    if(h%10>=5)
    {
        h+=step/2;
    }
    pVolume.fillRect(0,0,W,H,Qt::white);
    pTempIn.fillRect(0,0,W,H,Qt::white);
    pTempOut.fillRect(0,0,W,H,Qt::white);
    pHorLine.fillRect(0,0,W,15,QColor(239,235,222));
    int i,j=0;
    for(i=-startIndex;i<W;i+=100)
    {
        if(i>0)
        {
            pHorLine.drawText(i,13,QString::number(j));
        }
        j++;
    }
    pVolume.setPen(QColor(225,225,225));
    pTempIn.setPen(QColor(225,225,225));
    pTempOut.setPen(QColor(225,225,225));
    for(i=step;i<h;i+=step)
    {
        pVolume.drawLine(0,h+i,W,h+i);
        pTempIn.drawLine(0,h+i,W,h+i);
        pTempOut.drawLine(0,h+i,W,h+i);
        pVolume.drawLine(0,h-i,W,h-i);
        pTempIn.drawLine(0,h-i,W,h-i);
        pTempOut.drawLine(0,h-i,W,h-i);
    }
    for(i=10;i<W;i+=10)
    {
        pVolume.drawLine(i,0,i,h<<1);
        pTempIn.drawLine(i,0,i,h<<1);
        pTempOut.drawLine(i,0,i,h<<1);
    }
    pVolume.setPen(QColor(0,0,0));
    pTempIn.setPen(QColor(0,0,0));
    pTempOut.setPen(QColor(0,0,0));
    pVolume.drawLine(0,h,W,h);
    pTempIn.drawLine(0,h,W,h);
    pTempOut.drawLine(0,h,W,h);
    pVolume.setPen(QColor(255,0,0));
    float k = (float)h/8000;
    float k1 = (float)h/800;
    for(i=0;i<W;i++)
    {
        if(i+startIndex>=endIndex)break;
        pVolume.drawLine(i,h-k1*volume[i+startIndex],i+1,h-k1*volume[i+startIndex+1]);
        pTempIn.drawLine(i,h-k*tempIn[i+startIndex],i+1,h-k*tempIn[i+startIndex+1]);
        pTempOut.drawLine(i,h-k*tempOut[i+startIndex],i+1,h-k*tempOut[i+startIndex+1]);
        time = i+startIndex+1;
    }

    int min,sec,msec,lsec;
    QString t;
    time=time/10;
    lsec = time/10;
    min = lsec/60;
    sec= lsec - min*60;
    msec = time-sec*10-min*600;
    t.append("0"+QString::number(min)+":");
    if(sec<10)
    {
        t.append("0"+QString::number(sec)+":");
    }
    else
    {
        t.append(QString::number(sec)+":");
    }
    t.append(QString::number(msec));
    ui->timeLabel->setText(t);

    ui->gVolume->setPixmap(bVolume);
    ui->gTempIn->setPixmap(bTempIn);
    ui->gTempOut->setPixmap(bTempOut);
    ui->horisontLine->setPixmap(bHorLine);
    if(recordingStarted)
    {
        ui->horizontalScrollBar->setEnabled(false);
        ui->horizontalScrollBar->setMaximum(startIndex/10);
        ui->horizontalScrollBar->setValue(startIndex/10);
    }
}

void TSView::scrollValueChanged(int val)
{
    curveBuffer->setStartIndex(val*10);
    curveBuffer->setEnd(val*10+W);
    plotNow();
}

void TSView::f1()
{
    curveBuffer->append(0,1000,-1000);
}

void TSView::showNewResearchDialog(TSPatientProfileModel *model)
{
    QDialog *newResearchDialog = new QDialog;
    initPatientProfileUi();
    patientProfileUi->setupUi(newResearchDialog);
    patientProfileUi->idEdit->bindWithString(&model->id);
    patientProfileUi->nameEdit->bindWithString(&model->name);
    patientProfileUi->snEdit->bindWithString(&model->sname);
    patientProfileUi->inameEdit->bindWithString(&model->iname);
    patientProfileUi->ageEdit->bindWithString(&model->age);
    QStringList list;
    list.append(tr("Мужской"));
    list.append(tr("Женский"));
    patientProfileUi->genderComboBox->clear();
    patientProfileUi->genderComboBox->addItems(list);
    patientProfileUi->genderComboBox->bindWithString(&model->gender);
    patientProfileUi->mvlEdit->bindWithString(&model->mvl);
    connect(newResearchDialog,SIGNAL(accepted()),control,SLOT(newResearchAccepted()));
    newResearchDialog->exec();
}

void TSView::showEditPatientProfileDialog(TSPatientProfileModel *model)
{
    QDialog *editResearchDialog = new QDialog;
    initPatientProfileUi();
    patientProfileUi->setupUi(editResearchDialog);
    patientProfileUi->idEdit->bindWithString(&model->id);
    patientProfileUi->nameEdit->bindWithString(&model->name);
    patientProfileUi->snEdit->bindWithString(&model->sname);
    patientProfileUi->inameEdit->bindWithString(&model->iname);
    patientProfileUi->ageEdit->bindWithString(&model->age);
    QStringList list;
    list.append(tr("Мужской"));
    list.append(tr("Женский"));
    patientProfileUi->genderComboBox->clear();
    patientProfileUi->genderComboBox->addItems(list);
    patientProfileUi->genderComboBox->bindWithString(&model->gender);
    patientProfileUi->mvlEdit->bindWithString(&model->mvl);
    editResearchDialog->exec();
}

void TSView::showResearchwindow(TSCurveBuffer *model)
{
    curveBuffer = model;
    volume = curveBuffer->volume();
    tempIn = curveBuffer->tempIn();
    tempOut = curveBuffer->tempOut();
    H = ui->gVolume->height();
    W = ui->gVolume->width();
    bVolume = QPixmap(W,H);
    bTempIn = QPixmap(W,H);
    bTempOut = QPixmap(W,H);
    bHorLine = QPixmap(W,15);
    pVolume.begin(&bVolume);
    pTempIn.begin(&bTempIn);
    pTempOut.begin(&bTempOut);
    pHorLine.begin(&bHorLine);
    screenLimit = W;
    curveBuffer->setScreenLimit(W);
    startIndex = 0;
    ui->cbStart->setVisible(true);
    ui->cbStop->setVisible(true);
    ui->timeLabel->setVisible(true);
    connect(ui->cbStart,SIGNAL(clicked()),control,SLOT(startRecordingRequested()));
    connect(ui->cbStop,SIGNAL(clicked()),control,SLOT(stopRecoringRequested()));
    connect(&plotingTimer,SIGNAL(timeout()),this,SLOT(plotNow()));
    connect(&timer1,SIGNAL(timeout()),this,SLOT(f1()));
    time=0;
}

void TSView::startRecording()
{
    timer1.start(10);
    plotingTimer.start(100);
    recordingStarted = true;
    ui->cbStart->setEnabled(false);
    ui->cbStop->setEnabled(true);
}

void TSView::stopRecording()
{
    ui->horizontalScrollBar->setEnabled(true);
    connect(ui->horizontalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(scrollValueChanged(int)));
    plotingTimer.stop();
    recordingStarted = false;
    ui->cbStop->setEnabled(false);
}

void TSView::showCalibrateionDialog(TSCalibrateDialogModel *model)
{
    if(!calibrateDialogUi)
    {
        calibrateDialog = new QDialog(this);
        calibrateDialogUi = new Ui::TSCalibrateDialog;
        calibrateDialogUi->setupUi(calibrateDialog);
        connect(calibrateDialogUi->calibrateVolume,SIGNAL(clicked()),control,SLOT(startVolumeCalibrationRequested()));
        calibrateDialog->show();
    }
    else
    {
        calibrateDialogUi->volumeEdit->setText(QString::number(model->colibrateVolume));
        calibrateDialog->show();
    }
}

void TSView::showModelMessage(QString mess)
{
    message.setStandardButtons(QMessageBox::NoButton);
    message.setText(mess);
    message.setWindowTitle(tr("Сообщение"));
    message.show();
}

void TSView::closeModelMessage()
{
    message.hide();
}
