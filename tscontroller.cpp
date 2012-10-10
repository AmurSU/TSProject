#include "tscontroller.h"
#include "ui_tsview.h"
#include "ui_analizeResults.h"
#include <QtSql/QSqlRecord>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include "tools/tsvalidationtools.h"
#include <QSqlDatabase>
#include <QTranslator>
#include <QApplication>
#include <QResizeEvent>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime>
#include <QDate>
#include <math.h>
#include <QDir>
#include <tsanalitics.h>
#include <QTableWidget>

TSController::TSController(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::TSView)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("CP-1251"));
    QMainWindow *m = new QMainWindow();

    /* w = new Ui::MainWindow();
    w->setupUi(m);

    m->show();*/
    ui->setupUi(this);
    ui->mainBox->setCurrentIndex(0);
    currentAction = NoAction;
    curveBuffer = new TSCurveBuffer();
    readerThread = new TSReaderThread(curveBuffer);
    recordingStarted = false;
    tempInInterval = curveBuffer->getTempInInterval();
    tempInAdaptive = 1.0;
    tempOutInterval = curveBuffer->getTempOutInterval();
    tempOutAdaptive = 1.0;
    scaleScroll[0]=1;scaleScroll[1]=3;scaleScroll[2]=5;scaleScroll[3]=7;scaleScroll[4]=9;
    ui->managmentBox->setVisible(false);
    ui->vertLabel->setVisible(false);
    ui->horLabel->setVisible(false);
    ui->tempInScaleSlider->setVisible(false);
    ui->tempOutScaleSlider->setVisible(false);
    ui->volumeScaleSlider->setVisible(false);
    patientsConnection = QSqlDatabase::addDatabase("QSQLITE","Patients");
    patientsConnection.setDatabaseName("commondb\\common.db");
    if(!patientsConnection.open())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Ошибка"));
        msgBox.setText(tr("Произошла ошибка.\nОбратитесь к разработчикам.\nКод: 00001.\nПрограмма будет завершена."));
        msgBox.exec();
        QApplication::exit(0);
    }
    patientsModel = new TSPatients(patientsConnection);
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(editPatientProfile()));
    connect(ui->saveProfileButton,SIGNAL(clicked()),this,SLOT(savePatientProfile()));
    connect(ui->ignoreCalibrateButton,SIGNAL(clicked()),this,SLOT(rejectColibration()));
    connect(ui->volumeCalibrateButton,SIGNAL(clicked()),this,SLOT(calibrateVolume()));
    connect(ui->startExam,SIGNAL(clicked()),this,SLOT(startExam()));
    connect(&plotingTimer,SIGNAL(timeout()),this,SLOT(plotNow()));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openPatientList()));
    connect(ui->nameFilterEdit,SIGNAL(textChanged(QString)),this,SLOT(completePatientName(QString)));
    connect(ui->patientsTableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(openPatientProfile(QModelIndex)));
    connect(curveBuffer,SIGNAL(updateAverageData(int,int,int,int)),this,SLOT(showAverageData(int,int,int,int)));
    connect(ui->fName,SIGNAL(editingFinished()),this,SLOT(completePatientId()));
    connect(ui->sName,SIGNAL(editingFinished()),this,SLOT(completePatientId()));
    connect(ui->fdName,SIGNAL(editingFinished()),this,SLOT(completePatientId()));
    connect(ui->date,SIGNAL(editingFinished()),this,SLOT(completePatientId()));
    connect(ui->editProfileButton,SIGNAL(clicked()),this,SLOT(editPatientProfile()));
    connect(ui->rejectProfileButton,SIGNAL(clicked()),this,SLOT(rejectPatientProfile()));
    connect(ui->horizontalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(scrollGraphics(int)));
    connect(ui->newExamButton,SIGNAL(clicked()),this,SLOT(createNewExam()));
    connect(ui->stopExam,SIGNAL(clicked()),this,SLOT(stopExam()));
    connect(ui->examsTableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(openExam(QModelIndex)));
    connect(ui->tempInScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(scaleTempIn(int)));
    connect(ui->tempOutScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(scaleTempOut(int)));
    connect(ui->volumeScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(scaleVolume(int)));
    connect(ui->horScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(scaleForHorizontal(int)));
    connect(ui->horScaleSlider,SIGNAL(rangeChanged(int,int)),this,SLOT(changeScrollBarAfterScaling(int,int)));
    connect(ui->tempInScroll,SIGNAL(valueChanged(int)),this,SLOT(changeTempInScrollValue(int)));
    connect(ui->breakExamButton,SIGNAL(clicked()),this,SLOT(breakExam()));
    connect(ui->resultsButton,SIGNAL(clicked()),this,SLOT(processDataParams()));
    ui->resultsButton->setEnabled(true);
    ui->backPatientProfileButton->installEventFilter(this);
    ui->backPatientListButton->installEventFilter(this);
    ui->backCallibrateButton->installEventFilter(this);
    ui->backExamButton->installEventFilter(this);
    this->processDataParams();
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
        ui->idEdit->clear();
        ui->mGenderRadio->setChecked(true);
        break;
    }
    case 3:
    {
        currentAction = EditPatientProfileAction;
        QSqlRecord record = patientsModel->record(0);
        ui->fName->setText(record.value("fname").toString());
        ui->sName->setText(record.value("sname").toString());
        ui->fdName->setText(record.value("fdname").toString());
        ui->mvl->setText(record.value("mvl").toString());
        QStringList d = record.value("birth_date").toString().split("-");
        QString date = d.at(2)+"-"+d.at(1)+"-"+d.at(0);
        ui->date->setText(date);
        if(record.value("gender").toString()==tr("м"))
            ui->mGenderRadio->setChecked(true);
        else
            ui->fGenderRadio->setChecked(true);
        break;
    }
    default: break;
    }
    ui->mainBox->setCurrentIndex(1);
}

void TSController::savePatientProfile()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Ошибка"));
    msgBox.setText(tr("Неправельный ввод данных."));
    QSqlRecord record;
    record = patientsModel->record();
    record.setValue("sname",ui->sName->text().toUpper());
    record.setValue("fname",ui->fName->text().toUpper());
    record.setValue("fdname", ui->fdName->text().toUpper());
    record.setValue("code",ui->idEdit->text().toUpper());
    record.setValue("mvl",ui->mvl->text().toUpper());
    switch(ui->mGenderRadio->isChecked())
    {
    case 0: {record.setValue("genger",tr("ж")); break;}
    case 1: {record.setValue("genger",tr("ж")); break;}
    }
    QStringList d = ui->date->text().split("-");
    QString date = d.at(2)+"-"+d.at(1)+"-"+d.at(0);
    record.setValue("birth_date",date);
    /*
    QString string;
    string = ui->sName->text().toUpper();
    if(TSValidationTools::isNameString(string)==false)
    {
        msgBox.setInformativeText(tr("Поле фамилия не должно содержать ничего кроме букв русского алфавита"));
        msgBox.exec();
        return;
    }
    else
        record.setValue("sname",string);
    string = ui->fName->text().toUpper();
    if(TSValidationTools::isNameString(string)==false)
    {
        msgBox.setInformativeText(tr("Поле имя не должно содержать ничего кроме букв русского алфавита"));
        msgBox.exec();
        return;
    }
    else
        record.setValue("fname",string);
    string = ui->fdName->text().toUpper();
    if(TSValidationTools::isNameString(string)==false)
    {
        msgBox.setInformativeText(tr("Поле отчество не должна содержать ничего кроме букв русского алфавита"));
        msgBox.exec();
        return;
    }
    else
        record.setValue("fdname",string);
    string = ui->mvl->text().toUpper();
    if(TSValidationTools::isInt(string)==false)
    {
        msgBox.setInformativeText(tr("Поле МВЛ должно содержать целое число"));
        msgBox.exec();
        return;
    }
    else
        record.setValue("mvl",string);
    if(ui->mGenderRadio->isChecked())
        record.setValue("gender",tr("м"));
    if(ui->fGenderRadio->isChecked())
        record.setValue("gender",tr("ж"));
    QStringList d = ui->date->text().split("-");
    QString date = d.at(2)+"-"+d.at(1)+"-"+d.at(0);
    record.setValue("birth_date",date);
    record.setValue("code",ui->idEdit->text());
    */

    switch(currentAction)
    {
    case CreatePatientProfileAction:
    {
        if(!patientsModel->insertRecord(-1,record))
        {
            qDebug()<<patientsModel->lastError().text();
            return;
        }
        record = patientsModel->record(patientsModel->rowCount()-1);

        /* TODO вынести создание базы в отдельный метод */
        /*
            examinationsConnection = QSqlDatabase::addDatabase("QSQLITE","ExamConnection");
            examinationsConnection.setDatabaseName("privatedb\\"+record.value("id").toString()+"_"+
                                                   record.value("code").toString()+".db");
            if(!examinationsConnection.open())
            {
                qDebug()<<examinationsConnection.lastError().text();
                patientsModel->removeRow(patientsModel->rowCount()-1);
                QMessageBox msgBox(this);
                msgBox.setWindowTitle(tr("Ошибка"));
                msgBox.setText(tr("Произошла ошибка. Обратитесь к разработчикам. Код: 00002"));
                msgBox.exec();
                ui->mainBox->setCurrentIndex(0);
                return;
            }
            QSqlQuery q(examinationsConnection);
            q.prepare("CREATE TABLE `examinations` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT,`date` DATE,`time` TIME,`indication` TEXT,`diagnosis` TEXT,`nurse` VARCHAR(50),`doctor` VARCHAR(50),`tempOut` TEXT,`tempIn` TEXT,`volume`  TEXT);");
            if(!q.exec())
            {
                qDebug()<<q.lastError().text();
            }
            */
        /* TODO */
        openPrivateDB(record);
        QSqlQuery q(examinationsConnection);
        q.prepare("CREATE TABLE `examinations` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT,`date` DATE,`time` TIME,`indication` TEXT,`diagnosis` TEXT,`nurse` VARCHAR(50),`doctor` VARCHAR(50),`tempOut` TEXT,`tempIn` TEXT,`volume`  TEXT);");
        if(!q.exec())
        {
            qDebug()<<q.lastError().text();
        }
        patientsModel->setFilter("id="+record.value("id").toString());
        ui->patientPageLabel->setText(tr("Пациент: ")+record.value("sname").toString()+" "
                                      +record.value("fname").toString()+" "+record.value("fdname").toString());
        examinationsModel = new TSExaminations(examinationsConnection);
        ui->examsTableView->setModel(examinationsModel);
        ui->examsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->examsTableView->setColumnHidden(0,true);
        for(int i=2;i<9;i++)
        {
            ui->examsTableView->setColumnHidden(i,true);
        }
        ui->mainBox->setCurrentIndex(3);
        ui->examsTableView->horizontalHeader()->setDefaultSectionSize(ui->examsTableView->width()/2-1);
        //currentAction = NoAction;
        break;
    }
    case EditPatientProfileAction:
    {

        if(!patientsModel->setRecord(0,record))
        {
            qDebug()<<patientsModel->lastError().text();
            return;
        }
        patientsModel->submitAll();
        patientsModel->select();
        ui->mainBox->setCurrentIndex(2);
        currentAction = NoAction;
        break;
    }
    default: break;
    }
}

void TSController::rejectPatientProfile()
{
    switch(currentAction)
    {
    case CreatePatientProfileAction:
    {
        currentAction = NoAction;
        ui->mainBox->setCurrentIndex(0);
        break;
    }
    case EditPatientProfileAction:
    {
        currentAction = NoAction;
        ui->mainBox->setCurrentIndex(3);
        break;
    }
    default: break;
    }
}

void TSController::calibrateVolume()
{
    readerThread->setReadingType(ReadForVolZer);
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Предупреждение"));
    msgBox.setText(tr("Идет подготовка.\nПожалуйста подождите около 5 секунд..."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    readerThread->startRead();
    connect(readerThread,SIGNAL(done()),&msgBox,SLOT(accept()));
    switch(msgBox.exec())
    {
    case 1:
    {
        msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        msgBox.setText(tr("Подготовка завершина.\nНажмите кнопку Ok и качайте шприцем."));
        break;
    }
    default: break;
    }
    disconnect(&msgBox,SLOT(accept()));
    readerThread->stopRead();
    if(msgBox.exec()==QMessageBox::Ok)
    {
        ui->mainBox->setCurrentIndex(5);
        initPaintDevices();
        plotNow();
        ui->managmentBox->setVisible(true);
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
    if(pVolume.isActive()) pVolume.end();
    if(pTempIn.isActive()) pTempIn.end();
    if(pTempOut.isActive()) pTempOut.end();
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
    if(endIndex == 17999)plotingTimer.stop();
    int h = ui->gVolume->height()/2;
    int h1 = ui->gVolume->height()-5;
    int step = h/10;
    startIndex = endIndex - (W-35);
    if(startIndex < 0) startIndex = 0;
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
    pTempIn.drawLine(0,h+tempInZerPos*h,W,h+tempInZerPos*h);
    pTempOut.drawLine(0,h,W,h);
    pVolume.setPen(QColor(255,0,0));
    int* tinInt = curveBuffer->getTempInInterval();
    int* toutInt = curveBuffer->getTempOutInterval();
    int* volInt = curveBuffer->getVolumeInterval();
    //qDebug()<<tinInt[0]<<" "<<tinInt[1];
    float tempInK = tempInScaleRate*h;//*abs(tempInInterval[0]-tempInInterval[1])/abs(tinInt[1]-tinInt[0]);
    float tempOutK = tempOutScaleRate*h;

    /* TODO  решить проблему с увеличением адаптивного коэфициента  */

    /*if(tempInInterval[0]!=tinInt[0]||tempInInterval[1]!=tinInt[1])
    {
        float x = (float)(tempInInterval[1]-tempInInterval[0])/
                    (tinInt[1]-tinInt[0]);
        if(x>=2)
        {
            tempInAdaptive *= x/2;
        }
    }*/
    float tempInZ = h;// + tempInZerPos*h;
    float tempOutZ = h;// + tempInZerPos*h;
    tempInAdaptive = (float)10000/
            (tinInt[1]-tinInt[0]);
    tempOutAdaptive = (float)10000/
            (toutInt[1]-toutInt[0]);
    volumeAdaptive = (float)10000/(volInt[1]-volInt[0]);
    tempInZ = h + ceil((float)(tinInt[1]+tinInt[0])*tempInAdaptive*tempInK/2);
    tempOutZ = h + ceil((float)(toutInt[1]+toutInt[0])*tempOutAdaptive*tempOutK/2);
    //    delete tempInInterval;
    //    tempInInterval = tinInt;
    //    delete tempOutInterval;
    //    tempOutInterval = toutInt;
    float volumeK = volumeScaleRate*h;
    int j = 0, k = 1/horizontalStep;
    i=0;
    for(j=0;j<W-35;j+=10)
    {
        if(i+startIndex>=k*endIndex)break;
        pVolume.drawLine(j,h-volumeK*volumeAdaptive*volume[i+startIndex]
                         ,j+1,h-volumeK*volumeAdaptive*volume[i+startIndex+k]);
        pTempIn.drawLine(j,tempInZ-tempInK*tempInAdaptive*tempIn[i+startIndex]
                         ,j+1,tempInZ-tempInK*tempInAdaptive*tempIn[i+startIndex+k]);
        pTempOut.drawLine(j,tempOutZ-tempOutK*tempOutAdaptive*tempOut[i+startIndex]
                          ,j+1,tempOutZ-tempOutK*tempOutAdaptive*tempOut[i+startIndex+k]);
        i+=k*10;
    }
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

void TSController::startExam()
{
    recordingStarted = true;
    readerThread->setReadingType(ReadAll);
    readerThread->startRead();
    tempInScaleRate = 1.0/5000;
    tempOutScaleRate = 1.0/5000;
    volumeScaleRate = 1.0/5000;
    horizontalStep = 1.0;
    initPaintDevices();
    plotingTimer.start(100);
    ui->startExam->setEnabled(false);
}

void TSController::stopExam()
{
    if(recordingStarted)
    {
        plotingTimer.stop();
        readerThread->stopRead();
        QSqlRecord record = examinationsModel->record();
        int n = curveBuffer->end();
        QString val;
        int i;
        int *mass = curveBuffer->volume();
        for(i=0;i<n;i++)
        {
            val.append(QString::number(mass[i])+";");
        }
        record.setValue("volume",val);
        val.clear();
        mass = curveBuffer->tempIn();
        for(i=0;i<n;i++)
        {
            val.append(QString::number(mass[i])+";");
        }
        record.setValue("tempIn",val);
        val.clear();
        mass = curveBuffer->tempOut();
        for(i=0;i<n;i++)
        {
            val.append(QString::number(mass[i])+";");
        }
        record.setValue("tempOut",val);
        record.setValue("date",QDate::currentDate().toString("yyyy-MM-dd"));
        record.setValue("time",QTime::currentTime().toString("hh:mm"));
        if(!examinationsModel->insertRecord(-1,record))
            qDebug()<<"exam insertError";
        ui->horizontalScrollBar->setEnabled(true);
    }

    recordingStarted = false;
}

void TSController::openPatientList()
{
    patientsModel->setFilter("");
    patientsModel->select();
    ui->patientsTableView->setModel(patientsModel);
    ui->patientsTableView->setColumnHidden(0,true);
    ui->patientsTableView->setColumnHidden(5,true);
    ui->patientsTableView->setColumnHidden(6,true);
    ui->patientsTableView->setColumnHidden(8,true);
    ui->patientsTableView->setColumnHidden(9,true);
    ui->patientsTableView->setColumnHidden(10,true);
    ui->mainBox->setCurrentIndex(2);
    ui->patientsTableView->horizontalHeader()->setDefaultSectionSize((ui->patientsTableView->width()-2)/5);
    ui->patientsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->patientsTableView->setEditTriggers(QTableView::NoEditTriggers);
}

void TSController::completePatientName(QString string)
{
    string.toUpper();
    patientsModel->setFilter("sname like '"+string+"%' or code like '"+string+"%'");
    patientsModel->select();
}

void TSController::openPatientProfile(QModelIndex ind)
{

    QSqlRecord record;
    if(ind.row()==-1&&ind.column()==-1)
    {
        record = patientsModel->record(patientsModel->rowCount()-1);
    }
    else
    {
        record = patientsModel->record(ind.row());
    }

    ui->patientPageLabel->setText(tr("Пациент: ")+record.value("sname").toString()+" "+record.value("fname").toString()+
                                  " "+record.value("fdname").toString());
    openPrivateDB(record);
    /*
    examinationsConnection = QSqlDatabase::addDatabase("QSQLITE","ExamConnection");
    QDir d("privatedb");
    examinationsConnection.setDatabaseName("privatedb\\"+record.value("id").toString()
                                           +"_"+record.value("code").toString()+".db");
    if(!examinationsConnection.open()||!d.exists())
    {
        QMessageBox msg(this);
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Произошла потеря данных.\nВыполните восстановление."));
        msg.exec();
        ui->mainBox->setCurrentIndex(0);
        return;
    }
    */
    patientsModel->setFilter("id="+record.value("id").toString());
    examinationsModel = new TSExaminations(examinationsConnection);
    ui->examsTableView->setModel(examinationsModel);
    ui->examsTableView->setColumnHidden(0,true);
    for(int i=3;i<10;i++)
    {
        ui->examsTableView->setColumnHidden(i,true);
    }
    ui->examsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->mainBox->setCurrentIndex(3);
    ui->examsTableView->horizontalHeader()->setDefaultSectionSize((ui->examsTableView->width()-2)/3);
}


void TSController::showAverageData(int avgTempIn, int avgTempOut, int avgDO, int avgCHD)
{
    ui->volumeInfoLabel->setText(tr("ДО=")+QString::number(avgDO)+tr("\nЧД=")+QString::number(avgCHD));
    ui->tinInfoLabel->setText("Tin="+QString::number(avgTempIn));
    ui->toutInfolabel->setText("Tout="+QString::number(avgTempOut));
}


void TSController::completePatientId()
{
    QString id;
    if(ui->sName->text().length()>0)
        id.append(ui->sName->text().toUpper().at(0));
    if(ui->fName->text().length()>0)
        id.append(ui->fName->text().toUpper().at(0));
    if(ui->fdName->text().length()>0)
        id.append(ui->fdName->text().toUpper().at(0));
    id.append(ui->date->text().replace("-",""));
    ui->idEdit->setText(id);
}

void TSController::scrollGraphics(int value)
{
    curveBuffer->setEnd(W-35+value*horizontalStep*10);
    plotNow();
}

void TSController::createNewExam()
{
    ui->mainBox->setCurrentIndex(4);
}

void TSController::openExam(QModelIndex ind)
{
    QSqlRecord record = examinationsModel->record(ind.row());
    int volume[18000],tempin[18000], tempout[18000];
    int i;
    QStringList list = record.value("volume").toString().split(";");
    for(i=0;i<list.count();i++)
    {
        volume[i] = list.at(i).toInt();
    }
    list = record.value("tempIn").toString().split(";");
    for(i=0;i<list.count();i++)
    {
        tempin[i] = list.at(i).toInt();
    }
    list = record.value("tempOut").toString().split(";");
    for(i=0;i<list.count();i++)
    {
        tempout[i] = list.at(i).toInt();
        //qDebug()<<tempout[i];
    }
    curveBuffer->setValues(volume,tempin,tempout,list.count());
    //curveBuffer->setStartIndex(0);
    ui->mainBox->setCurrentIndex(5);
    ui->horizontalScrollBar->setMaximum((list.count()-ui->gVolume->width())/10);
    ui->horizontalScrollBar->setValue(0);
    ui->horizontalScrollBar->setEnabled(true);
    initPaintDevices();
    curveBuffer->setEnd(W-35);
    tempInScaleRate = 1.0/5000;
    tempOutScaleRate = 1.0/5000;
    volumeScaleRate = 4.0/5000;
    horizontalStep = 1.0;
    ui->managmentBox->setVisible(true);
    ui->managmentBox->setEnabled(true);
    plotNow();
}

void TSController::resizeEvent(QResizeEvent *evt)
{
    initPaintDevices();
    plotNow();
}

void TSController::scaleTempIn(int value)
{
    if(value>0)
    {
        ui->tempInScroll->setMinimum((-1)*scaleScroll[value-1]);
        ui->tempInScroll->setMaximum(scaleScroll[value-1]);
        ui->tempInScroll->setValue(0);
    }
    else
    {
        ui->tempInScroll->setMinimum(0);
        ui->tempInScroll->setMaximum(0);
        ui->tempInScroll->setValue(0);
    }
    value*=2;
    if(value<0)
    {
        tempInScaleRate = (-1.0)/value/5000;
    }
    if(value == 0)
    {
        tempInScaleRate = 1.0/5000;
    }
    if(value>0)
    {
        tempInScaleRate = (float)value/5000;
    }
    plotNow();
}

void TSController::scaleTempOut(int value)
{
    if(value>0)
    {
        ui->tempOutScroll->setMinimum((-1)*scaleScroll[value-1]);
        ui->tempOutScroll->setMaximum(scaleScroll[value-1]);
        ui->tempOutScroll->setValue(0);
    }
    else
    {
        ui->tempOutScroll->setMinimum(0);
        ui->tempOutScroll->setMaximum(0);
        ui->tempOutScroll->setValue(0);
    }
    value*=2;
    if(value<0)
    {
        tempOutScaleRate = (-1.0)/value/5000;
    }
    if(value == 0)
    {
        tempOutScaleRate = 1.0/5000;
    }
    if(value>0)
    {
        tempOutScaleRate = (float)value/5000;
    }
    plotNow();
}

void TSController::scaleVolume(int value)
{
    value*=2;
    if(value<0)
    {
        volumeScaleRate = (-4.0)/value/5000;
    }
    if(value == 0)
    {
        volumeScaleRate = 4.0/5000;
    }
    if(value>0)
    {
        volumeScaleRate = (float)value*4/5000;
    }
    plotNow();
}

void TSController::scaleForHorizontal(int value)
{
    value*=2;
    if(value!=0)
    {
        horizontalStep = (-1.0)/value;
        //curveBuffer->setEnd((-1)*value*W-35);
        curveBuffer->setEnd(W-35);
    }
    else
    {
        horizontalStep = 1.0;
        curveBuffer->setEnd(W-35);
    }
    plotNow();
}

void TSController::changeScrollBarAfterScaling(int before, int after)
{
    if(after%2)return;
    int val = ui->horizontalScrollBar->value();
    if(before>after)
    {
        ui->horizontalScrollBar->setMaximum(ui->horizontalScrollBar->maximum()/2);
        ui->horizontalScrollBar->setValue(val/2);
    }
    else
    {
        ui->horizontalScrollBar->setMaximum(ui->horizontalScrollBar->maximum()*2);
        ui->horizontalScrollBar->setValue(val*2);
    }
}

void TSController::changeTempInScrollValue(int value)
{
    tempInZerPos = (-1)*value;
    plotNow();
}

void TSController::changeTempOutScrollValue(int value)
{
    tempOutZerPos = (-1)*value;
    plotNow();
}

bool TSController::eventFilter(QObject *obj, QEvent *e)
{
    QMouseEvent *evt;
    if(e->type() == QEvent::MouseButtonPress) evt = static_cast<QMouseEvent*>(e);
    if(obj == ui->backPatientProfileButton && evt->button()==Qt::LeftButton)
    {
        if(currentAction == CreatePatientProfileAction)
            ui->mainBox->setCurrentIndex(0);
        if(currentAction == NoAction)
            ui->mainBox->setCurrentIndex(2);
    }
    if(obj == ui->backPatientListButton && evt->button()==Qt::LeftButton)
    {
        ui->mainBox->setCurrentIndex(0);
        patientsModel->setFilter("");
    }
    if(obj == ui->backCallibrateButton && evt->button()==Qt::LeftButton)
    {
        ui->mainBox->setCurrentIndex(3);
    }
    if(obj == ui->backExamButton && evt->button()==Qt::LeftButton)
    {
        ui->mainBox->setCurrentIndex(4);
        ui->managmentBox->setVisible(false);
        curveBuffer->setEnd(0);
    }
    return QObject::eventFilter(obj,e);
}

void TSController::openPrivateDB(QSqlRecord record)
{
    examinationsConnection = QSqlDatabase::addDatabase("QSQLITE","ExamConnection");
    QDir d;
    if(!d.cd("pravatedb"))
    {
        d.mkpath("privatedb");
    }
    d.setPath(d.path()+"\\privatedb");
    examinationsConnection.setDatabaseName(d.path()+"\\"+record.value("id").toString()+"_"+
                                           record.value("code").toString()+".db");
    if(!examinationsConnection.open())
    {
        qDebug()<<examinationsConnection.lastError().text();
        //patientsModel->removeRow(patientsModel->rowCount()-1);
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Ошибка"));
        msgBox.setText(tr("Произошла ошибка. Обратитесь к разработчикам. Код: 00002"));
        msgBox.exec();
        ui->mainBox->setCurrentIndex(0);
        return;
    }
}

QTableWidgetItem* TSController::getQTableWidgetItem(QVariant text){
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setData(0,text);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

    return item;
}

void TSController::breakExam()
{
    plotingTimer.stop();
    readerThread->stopRead();
}

void TSController::processDataParams(){

    qDebug()<<"this is result button !";
    QTableWidget *qtw = ui->resultsTable;
    qtw->setColumnCount(2);
    qtw->setRowCount(12);
    //qtw->resize(250,480);
    qtw->verticalHeader()->setVisible(false);
    qtw->setHorizontalHeaderLabels(QString(tr("Параметр; Значение")).split(";"));
    qtw->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tsanalitics* ga = new tsanalitics();
    int AvgExpirationSpeed=0, AvgInspirationSpeed=0, MaxExpirationSpeed=0, MaxInspirationSpeed=0, AvgExpirationTime=0, AvgInspirationTime=0,
            AvgRoundTime=0, AvgTempIn=0, AvgTempOut=0, AvgTempInMinusAvgTempOut=0, InspirationFrequency=0, MinuteInspirationVolume=0;
    int i=0;
    int *vo = curveBuffer->volume();
    for(i=0;i<curveBuffer->end();i++){
        ga->append(vo[i]);
    }
    ga->findExtremums();
    ga->deleteBadExtremums();
    AvgExpirationSpeed = ga->getAvgExpiratorySpeed();
    qtw->setItem(1,0,getQTableWidgetItem(tr("Средняя скорость выдоха")));
    qtw->setItem(1,1,getQTableWidgetItem(AvgExpirationSpeed));

    AvgInspirationSpeed = ga->getAvgInspiratorySpeed();
    qtw->setItem(2,0,getQTableWidgetItem(tr("Средняя скорость вдоха")));
    qtw->setItem(2,1,getQTableWidgetItem(AvgInspirationSpeed));

    MaxExpirationSpeed = ga->getMaxExpiratorySpeed();
    qtw->setItem(3,0,getQTableWidgetItem(tr("Максимальная скорость выдоха")));
    qtw->setItem(3,1,getQTableWidgetItem(MaxExpirationSpeed));

    MaxInspirationSpeed = ga->getMaxInspiratorySpeed();
    qtw->setItem(4,0,getQTableWidgetItem(tr("Максимальная скорость вдоха")));
    qtw->setItem(4,1,getQTableWidgetItem(MaxInspirationSpeed));

    AvgExpirationTime = ga->getAvgExpiratoryTime();
    qtw->setItem(5,0,getQTableWidgetItem(tr("Среднее время выдоха")));
    qtw->setItem(5,1,getQTableWidgetItem(AvgExpirationTime));

    AvgInspirationTime = ga->getAvgInspiratoryTime();
    qtw->setItem(6,0,getQTableWidgetItem(tr("Среднее время вдоха")));
    qtw->setItem(6,1,getQTableWidgetItem(AvgInspirationTime));

    AvgRoundTime = AvgExpirationTime+AvgInspirationTime;
    qtw->setItem(7,0,getQTableWidgetItem(tr("Средняя время цикла")));
    qtw->setItem(7,1,getQTableWidgetItem(AvgRoundTime));

    InspirationFrequency = ga->getFrequency();
    qtw->setItem(8,0,getQTableWidgetItem(tr("Частота дыхания")));
    qtw->setItem(8,1,getQTableWidgetItem(InspirationFrequency));

    MinuteInspirationVolume = ga->getBreathingVolume();
    qtw->setItem(9,0,getQTableWidgetItem(tr("Минутный объем дыхания")));
    qtw->setItem(9,1,getQTableWidgetItem(MinuteInspirationVolume));

    ga->clear();
    int *ti = curveBuffer->tempIn();
    for(i=0;i<curveBuffer->end();i++){
        ga->append(ti[i]);
    }
    ga->findExtremums();
    ga->deleteBadExtremums();
    AvgTempIn = ga->getMinAvgs();
    qtw->setItem(10,0,getQTableWidgetItem(tr("Средняя температура вдоха")));
    qtw->setItem(10,1,getQTableWidgetItem(AvgTempIn));

    ga->clear();
    int *to = curveBuffer->tempOut();
    for(i=0;i<curveBuffer->end();i++){
        ga->append(to[i]);
    }
    ga->findExtremums();
    ga->deleteBadExtremums();
    AvgTempOut = ga->getMaxAvgs();
    qtw->setItem(11,0,getQTableWidgetItem(tr("Средняя температура выдоха")));
    qtw->setItem(11,1,getQTableWidgetItem(AvgTempOut));

    AvgTempInMinusAvgTempOut = AvgTempOut-AvgTempIn;
    qtw->setItem(12,0,getQTableWidgetItem(tr("Средняя Твдоха-Средняя Твыдоха")));
    qtw->setItem(12,1,getQTableWidgetItem(AvgTempInMinusAvgTempOut));
    qtw->removeRow(0);
    qtw->show();
}
