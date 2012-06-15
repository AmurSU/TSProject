#include "tsview.h"
#include "ui_tsview.h"
#include "ui_patientprofile.h"
#include <QColor>
#include <QPalette>
#include <QTime>
#include <QFont>
#include <QString>
#include <QDebug>
#include <QDialog>

TSView::TSView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TSView)
{
    ui->setupUi(this);
    ui->graphicsPlotArea->setBackgroundRole(QPalette::Dark);
    ui->cbStart->setVisible(false);
    ui->cbStop->setVisible(false);
    showCurvesTimer = 0;
    graphPen.setColor(QColor(255,0,0));
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
    connect(ui->actionNewResearch,SIGNAL(triggered()),control,SLOT(viewNewResearchRequested()));
}

void TSView::drawCurves(CurvesSegnments s)
{
    int end = cbuffer->end();
    vol.drawLine(end-1,s.prevV,end,s.curV);
    tin.drawLine(end-1,s.prevTin,end,s.curTin);
    tout.drawLine(end-1,s.prevTout,end,s.curTout);
}

void TSView::showCurves()
{
    int end = cbuffer->end();
    ui->gVolume->setPixmap(volPixmap);
    ui->gTempIn->setPixmap(tinPixmap);
    ui->gTempOut->setPixmap(toutPixmap);
    if(end+35>=screenLimit)
    {
        ui->gVolume->setMinimumWidth(end+35);
        ui->gTempIn->setMinimumWidth(end+35);
        ui->gTempOut->setMinimumWidth(end+35);
        ui->horisontalLine->setMinimumWidth(end+35);
    }
}

void TSView::showRecordWindow(TSCurveBuffer *curves)
{
    showCurvesTimer = new QTimer;
    screenLimit = ui->gVolume->width();
    h=ui->gVolume->height();
    QPixmap hLinePxm(18000,15);
    QPainter hLineP(&hLinePxm);
    hLineP.setFont(QFont("Tahoma",8,8));
    hLineP.fillRect(0,0,18000,15,QColor(225,225,200));
    int i = 100,sec=1;
    QString time;
    for(i;i<18000;i+=100)
    {
        time=QString::number(sec);
        hLineP.drawText(i-time.length()*4,12,time);
        sec++;
    }
    hLineP.end();
    QPen netPen(QColor(210,210,210));
    QPen axisPen(QColor(100,100,100));
    axisPen.setWidth(2);
    volPixmap = QPixmap(18000,h);
    tinPixmap = QPixmap(18000,h);
    toutPixmap = QPixmap(18000,h);
    vol.begin(&volPixmap);
    vol.setPen(netPen);
    vol.fillRect(0,0,18000,h,Qt::white);
    tin.begin(&tinPixmap);
    tin.fillRect(0,0,18000,h,Qt::white);
    tin.setPen(netPen);
    tout.begin(&toutPixmap);
    tout.fillRect(0,0,18000,h,Qt::white);
    tout.setPen(netPen);
    for(i=10;i<18000;i+=10)
    {
        vol.drawLine(i,0,i,h);
        tin.drawLine(i,0,i,h);
        tout.drawLine(i,0,i,h);
    }
    for(i=10;i<h;i+=10)
    {
        vol.drawLine(0,i,18000,i);
        tin.drawLine(0,i,18000,i);
        tout.drawLine(0,i,18000,i);
    }
    int centre = h/20;
    centre*=10;
    vol.setPen(axisPen);
    vol.drawLine(0,centre,18000,centre);
    tin.setPen(axisPen);
    tin.drawLine(0,centre,18000,centre);
    tout.setPen(axisPen);
    tout.drawLine(0,centre,18000,centre);
    ui->cbStart->setVisible(true);
    ui->cbStop->setVisible(true);
    ui->horisontalLine->setPixmap(hLinePxm);
    ui->horisontalLine->setMaximumWidth(screenLimit);
    ui->gVolume->setPixmap(volPixmap);
    ui->gVolume->setMaximumWidth(screenLimit);
    ui->gTempIn->setPixmap(tinPixmap);
    ui->gTempIn->setMaximumWidth(screenLimit);
    ui->gTempOut->setPixmap(toutPixmap);
    ui->gTempOut->setMaximumWidth(screenLimit);
    cbuffer = curves;
    connect(cbuffer,SIGNAL(changed(CurvesSegnments)),this,SLOT(drawCurves(CurvesSegnments)));
    connect(showCurvesTimer,SIGNAL(timeout()),this,SLOT(showCurves()));
    connect(ui->cbStart,SIGNAL(clicked()),control,SLOT(listeningDeviceRequested()));
}

void TSView::showNewResearchDialog(TSPatientProfileModel *model)
{
    QDialog *newResearchDialog = new QDialog;
    patientProfileUi = new Ui::TSPatientProfile;
    patientProfileUi->setupUi(newResearchDialog);
    patientProfileUi->nameEdit->bindWithString(&model->name);
    patientProfileUi->snEdit->bindWithString(&model->sname);
    patientProfileUi->inameEdit->bindWithString(&model->iname);
    patientProfileUi->ageEdit->bindWithString(&model->age);
    patientProfileUi->mvlEdit->bindWithString(&model->mvl);
    QStringList list;
    list.append(tr("Мужской"));
    list.append(tr("Женский"));
    patientProfileUi->gendeComboBox->addItems(list);
    newResearchDialog->setWindowTitle(tr("Новое исследование"));
    connect(newResearchDialog,SIGNAL(accepted()),control,SLOT(viewNewResearchAccepted()));
    newResearchDialog->exec();
}

void TSView::startRealtimeViewing()
{
    showCurvesTimer->start(100);
}
