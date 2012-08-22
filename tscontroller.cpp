#include "tscontroller.h"
#include "ui_tsview.h"
#include <QtSql/QSqlRecord>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include "tools/tsvalidationtools.h"
#include <QSqlDatabase>
#include <QTranslator>
#include <QApplication>

TSController::TSController(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::TSView)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("CP-1251"));
    ui->setupUi(this);
    ui->mainBox->setCurrentIndex(0);
    currentAction = NoAction;
    curveBuffer = new TSCurveBuffer();
    readerThread = new TSReaderThread(curveBuffer);
    recordingStarted = false;
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
            q.prepare("CREATE TABLE `examinations` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT,`date` DATE,`indication` TEXT,`diagnosis` TEXT,`nurse` VARCHAR(50),`doctor` VARCHAR(50),`tempOut` TEXT,`tempIn` TEXT,`volume`  TEXT);");
            if(!q.exec())
            {
                qDebug()<<q.lastError().text();
            }
            patientsModel->setFilter("id="+record.value("id").toString());
            ui->patientPageLabel->setText(tr("Пациент: ")+record.value("sname").toString()+" "
                                          +record.value("fname").toString()+" "+record.value("fdname").toString());
            examinationsModel = new TSExaminations(examinationsConnection);
            ui->examsTableView->setModel(examinationsModel);
            ui->mainBox->setCurrentIndex(3);
            currentAction = NoAction;
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
    readerThread->setReadingType(ReadForVol);
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
    float k = (float)h/5000;
    float k1 = (float)h/2000;
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
    initPaintDevices();
    plotingTimer.start(100);
    ui->startExam->setEnabled(false);
}

void TSController::stopExam()
{
    recordingStarted = false;
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
    if(!examinationsModel->insertRecord(-1,record))
        qDebug()<<"exam insertError";
    ui->horizontalScrollBar->setEnabled(true);
}

void TSController::openPatientList()
{
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
    QSqlRecord record = patientsModel->record(ind.row());
    patientsModel->setFilter("id="+record.value("id").toString());
    ui->patientPageLabel->setText(tr("Пациент: ")+record.value("sname").toString()+" "+record.value("fname").toString()+
                                  " "+record.value("fdname").toString());
    examinationsConnection = QSqlDatabase::addDatabase("QSQLITE","ExamConnection");
    examinationsConnection.setDatabaseName("privatedb\\"+record.value("id").toString()
                                           +"_"+record.value("code").toString()+".db");
    if(!examinationsConnection.open())
        qDebug()<<"can`t open ExamConnection";
    examinationsModel = new TSExaminations(examinationsConnection);
    ui->examsTableView->setModel(examinationsModel);
    ui->mainBox->setCurrentIndex(3);
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
    curveBuffer->setStartIndex(value*10);
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
    //qDebug()<<curveBuffer->end();
    curveBuffer->setStartIndex(0);
    ui->mainBox->setCurrentIndex(5);
    ui->horizontalScrollBar->setMaximum(list.count()/10-ui->gVolume->width()/10-3);
    //qDebug()<<ui->horizontalScrollBar->maximum();
    ui->horizontalScrollBar->setValue(0);
    ui->horizontalScrollBar->setEnabled(true);
    initPaintDevices();
    plotNow();
}
