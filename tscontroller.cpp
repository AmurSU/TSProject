#include "tscontroller.h"
#include "ui_tsview.h"
#include <QtSql/QSqlRecord>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include "tools/tsvalidationtools.h"

TSController::TSController(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::TSView)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("CP-1251"));
    ui->setupUi(this);
    ui->mainBox->setCurrentIndex(4);
    currentAction = NoAction;
    curveBuffer = new TSCurveBuffer();
    readerThread = new TSReaderThread(curveBuffer);
    msgBox = new QMessageBox(this);
    recordingStarted =false;
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(editPatientProfile()));
    connect(ui->saveProfileButton,SIGNAL(clicked()),this,SLOT(savePatientProfile()));
    connect(ui->ignoreCalibrateButton,SIGNAL(clicked()),this,SLOT(rejectColibration()));
    connect(ui->volumeCalibrateButton,SIGNAL(clicked()),this,SLOT(calibrateVolume()));
    connect(ui->startResearch,SIGNAL(clicked()),this,SLOT(startExam()));
    connect(&plotingTimer,SIGNAL(timeout()),this,SLOT(plotNow()));
}

TSController::~TSController()
{
    delete ui;
}

void TSController::incCurrentIndex()
{
    ui->mainBox->setCurrentIndex(ui->mainBox->currentIndex()+1);
}

void TSController::decCurrentIndex()
{
    ui->mainBox->setCurrentIndex(ui->mainBox->currentIndex()-1);
}

void TSController::editPatientProfile()
{
    switch(ui->mainBox->currentIndex())
    {
        case 0:
        {
            currentAction = CreatePatientProfileAction;
            ui->fName->clear();
            ui->sName->clear();
            ui->fdName->clear();
            ui->mvl->clear();
            ui->date->clear();
            ui->mGenderRadio->setChecked(true);
            break;
        }
        default: break;
    }
    ui->mainBox->setCurrentIndex(1);
}

void TSController::savePatientProfile()
{
    msgBox->setWindowTitle(tr("Ошибка"));
    msgBox->setText(tr("Неправельный ввод данных."));
    switch(currentAction)
    {
        case CreatePatientProfileAction:
        {
            //some insert code
            QVariant value;
            QSqlRecord record;
            QString gender;
            if((value = TSValidationTools::isNameString(ui->sName->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле фамилия не должно содержать ничего кроме букв русского алфавита"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if((value = TSValidationTools::isNameString(ui->fName->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле имя не должно содержать ничего кроме букв русского алфавита"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if((value = TSValidationTools::isNameString(ui->fdName->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле отчество не должна содержать ничего кроме букв русского алфавита"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if((value = TSValidationTools::isInt(ui->mvl->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле МВЛ должно содержать целое число"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if(ui->mGenderRadio->isChecked())
            {

            }
            if(ui->fGenderRadio->isChecked())
            {

            }
            ui->mainBox->setCurrentIndex(4);
            break;
        }

        case EditPatientProfileAction:
        {
            //some update code
            break;
        }
        default: break;
    }
    delete msgBox;
}

void TSController::rejectPatientProfile()
{

}

void TSController::calibrateVolume()
{
    readerThread->setReadingType(ReadForVol);
    msgBox->setWindowTitle(tr("Предупреждение"));
    msgBox->setText(tr("Идет подготовка.\nПожалуйста подождите около 5 секунд..."));
    msgBox->setStandardButtons(QMessageBox::Ok);
    readerThread->startRead();
    connect(readerThread,SIGNAL(done()),msgBox,SLOT(accept()));
    switch(msgBox->exec())
    {
        case 1:
        {
            msgBox->setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            msgBox->setText(tr("Подготовка завершина.\nНажмите кнопку Ok и качайте шприцем."));
            break;
        }
        default: break;
    }
    disconnect(msgBox,SLOT(accept()));
    readerThread->stopRead();
    if(msgBox->exec()==QMessageBox::Ok)
    {
        //qDebug()<<"continue colibrate";
        ui->mainBox->setCurrentIndex(5);
        ui->managmentBox->setEnabled(true);
    }
}

void TSController::calibrateTemperature()
{

}

void TSController::rejectColibration()
{
    ui->mainBox->setCurrentIndex(5);
    ui->managmentBox->setEnabled(true);
}

void TSController::initPaintDevices()
{
    H = ui->gVolume->height();
    W = ui->gVolume->width();
    bVolume = QPixmap(W,H);
    bTempIn = QPixmap(W,H);
    bTempOut = QPixmap(W,H);
    pVolume.begin(&bVolume);
    pTempIn.begin(&bTempIn);
    pTempOut.begin(&bTempOut);
    screenLimit = W;
    curveBuffer->setScreenLimit(W);
    volume = curveBuffer->volume();
    tempIn = curveBuffer->tempIn();
    tempOut = curveBuffer->tempOut();
    startIndex = 0;
}

void TSController::threadFinished()
{
    qDebug()<<"tread is finished";
}

void TSController::plotNow()
{
    int endIndex = curveBuffer->end();
    int h = ui->gVolume->height()/2;
    int h1 = ui->gVolume->height()-5;
    if(endIndex == 17999)plotingTimer.stop();
    startIndex = curveBuffer->startIndex();
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
    pTempOut.setPen(curveBuffer->toutColor);
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
    }
    ui->gVolume->setPixmap(bVolume);
    ui->gTempIn->setPixmap(bTempIn);
    ui->gTempOut->setPixmap(bTempOut);
    /*if(recordingStarted)
    {
        ui->horizontalScrollBar->setEnabled(false);
        ui->horizontalScrollBar->setMaximum(startIndex/10);
        ui->horizontalScrollBar->setValue(startIndex/10);
    }*/
}

void TSController::startExam()
{
    recordingStarted = true;
    //plotingTimer.start(100);
    readerThread->setReadingType(ReadAll);
    readerThread->startRead();
    initPaintDevices();
    plotingTimer.start(100);
    ui->startResearch->setEnabled(false);
}
