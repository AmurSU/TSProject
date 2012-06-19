#include "tsview.h"
#include "ui_tsview.h"
#include "ui_patientprofile.h"
#include <QDebug>
#include <QDialog>
#include <QStringList>

TSView::TSView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TSView)
{
    ui->setupUi(this);
    patientProfileUi = 0;
    ui->cbStart->setVisible(false);
    ui->cbStop->setVisible(false);
    recordingStarted = false;
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
    if(endIndex == 17999)plotingTimer.stop();
    if(endIndex+35>=screenLimit)
    {
        screenLimit+=10;
        startIndex+=10;
    }
    int step = h/10;
    if(h%10>=5)
    {
        h+=step/2;
    }
    pVolume.fillRect(0,0,W,H,Qt::white);
    pTempIn.fillRect(0,0,W,H,Qt::white);
    pTempOut.fillRect(0,0,W,H,Qt::white);
    int i;
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
    float k = (float)h/8000;
    for(i=0;i<screenLimit;i++)
    {
        if(i+startIndex>=endIndex)break;
        pVolume.drawLine(i,h-k*volume[i+startIndex],i+1,h-k*volume[i+startIndex+1]);
        pTempIn.drawLine(i,h-k*tempIn[i+startIndex],i+1,h-k*tempIn[i+startIndex+1]);
        pTempOut.drawLine(i,h-k*tempOut[i+startIndex],i+1,h-k*tempOut[i+startIndex+1]);
    }
    pVolume.drawLine(0,h,W,h);
    pTempIn.drawLine(0,h,W,h);
    pTempOut.drawLine(0,h,W,h);
    ui->gVolume->setPixmap(bVolume);
    ui->gTempIn->setPixmap(bTempIn);
    ui->gTempOut->setPixmap(bTempOut);
    if(recordingStarted)
    {
        ui->horizontalScrollBar->setEnabled(false);
        ui->horizontalScrollBar->setMaximum(startIndex/10);
        ui->horizontalScrollBar->setValue(startIndex/10);
    }
}

void TSView::scrollValueChanged(int val)
{
    startIndex = val*10;
    plotNow();
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
    pVolume.begin(&bVolume);
    pTempIn.begin(&bTempIn);
    pTempOut.begin(&bTempOut);
    screenLimit = W;
    startIndex = 0;
    ui->cbStart->setVisible(true);
    ui->cbStop->setVisible(true);
    connect(ui->cbStart,SIGNAL(clicked()),control,SLOT(startRecordingRequested()));
    connect(ui->cbStop,SIGNAL(clicked()),control,SLOT(stopRecoringRequested()));
    connect(&plotingTimer,SIGNAL(timeout()),this,SLOT(plotNow()));
}

void TSView::startRecording()
{
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
