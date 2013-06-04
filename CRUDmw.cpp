#include "CRUDmw.h"
#include "ui_CRUDmw.h"
#include "ui_dialog.h"
#include "tsmodel.h"
#include "tscontroller.h"
#include "bdgod.h"
#include <QDebug>


QSqlDatabase MainWindow::dbConnect()
{
    // Connecting to database
    QSqlDatabase baseConnector = QSqlDatabase::addDatabase("QSQLITE");
    baseConnector.setDatabaseName("TSProject.sqlite");
    if(baseConnector.open()) {
        qDebug()<<"Connected to database";
    } else {
        QMessageBox msg;
        msg.setText("РќРµ СѓРґР°Р»РѕСЃСЊ РїРѕРґСЃРѕРµРґРёРЅРёС‚СЃСЏ Рє Р±Р°Р·Рµ РґР°РЅРЅС‹С…. РџСЂРѕРІРµСЂСЊС‚Рµ РїСЂР°РІР° РґРѕСЃС‚СѓРїР°.");
        msg.show();
        qDebug()<<"Not connected to database";
       }
    return baseConnector;

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QSqlDatabase baseConnector = dbConnect();
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    // Setting up models and table views
    // Patient
    patientsModel = new QSqlTableModel(this,baseConnector);
    patientsModel->setTable("patients");
    patientsModel->setEditStrategy(QSqlTableModel::OnRowChange);
    patientsModel->select();    
    patientsModel->setHeaderData(0, Qt::Horizontal, QString("ID"));
    patientsModel->setHeaderData(1, Qt::Horizontal, QString("РљРѕРґ"));
    patientsModel->setHeaderData(2, Qt::Horizontal, QString("Р¤Р°РјРёР»РёСЏ"));
    patientsModel->setHeaderData(3, Qt::Horizontal, QString("РРјСЏ"));
    patientsModel->setHeaderData(4, Qt::Horizontal, QString("Р РѕР¶РґРµРЅРёРµ"));
    patientsModel->setHeaderData(5, Qt::Horizontal, QString("Р РѕСЃС‚"));
    patientsModel->setHeaderData(6, Qt::Horizontal, QString("Р’РµСЃ"));
    patientsModel->setHeaderData(7, Qt::Horizontal, QString("РџРѕР»"));
    patientsModel->setHeaderData(8, Qt::Horizontal, QString("РљСѓСЂРёС‚?"));
    ui->patientsView->setModel(patientsModel);
    ui->patientsView->setColumnHidden(0,true);
    ui->patientsView->setColumnHidden(5,true);
    ui->patientsView->setColumnHidden(6,true);
    ui->patientsView->setColumnHidden(7,true);
    ui->patientsView->setColumnHidden(8,true);
    ui->patientsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->patientsView->setEditTriggers(QTableView::NoEditTriggers);
    connect(ui->patientsView,SIGNAL(doubleClicked(QModelIndex)),this, SLOT(on_rowOfPTDoubleClicked(QModelIndex)));
    connect(ui->newExamButton,SIGNAL(clicked()),this,SLOT(on_newExamButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newExamButton_clicked()
{
    //BDgod index;

    QModelIndex ind = indexx;
    QString pid = patientsModel->record(ind.row()).value("id").toString();
    QString name = patientsModel->record(ind.row()).value("name").toString();
    QString surname = patientsModel->record(ind.row()).value("surname").toString();
    BDgod::CreateNewDatabase(pid,name,surname);
    qDebug()<<"newExam Click catched";
    this->close();
}

void MainWindow::on_rowOfPTDoubleClicked(QModelIndex ind)
{
    indexx = ind;
    //BDgod index;
    //index.CacheInd(ind);
    //QSqlDatabase baseConnector = dbConnect();
    ui->stackedWidget->setCurrentIndex(1);
    // Examinations
    examsModel = new QSqlTableModel();
    examsModel->setTable("examinations");
    examsModel->setEditStrategy(QSqlTableModel::OnRowChange);
    if (ind.isValid()) {
        QString pid = patientsModel->record(ind.row()).value("id").toString();
        examsModel->setFilter(QString("`patient_id` LIKE '%1'").arg(pid));
        examsModel->select();
    }
    qDebug()<<"Filter payments by account "<<examsModel->filter();
    examsModel->setHeaderData(0, Qt::Horizontal, QString("ID"));
    examsModel->setHeaderData(1, Qt::Horizontal, QString("РљРѕРґ"));
    examsModel->setHeaderData(2, Qt::Horizontal, QString("Р”Р°С‚Р°"));
    examsModel->setHeaderData(3, Qt::Horizontal, QString("РџРѕРєР°Р·Р°РЅРёСЏ"));
    examsModel->setHeaderData(4, Qt::Horizontal, QString("Р”РёР°РіРЅРѕР·"));
    examsModel->setHeaderData(5, Qt::Horizontal, QString("РњРµРґСЃРµСЃС‚СЂР°"));
    examsModel->setHeaderData(6, Qt::Horizontal, QString("Р’СЂР°С‡"));
    examsModel->setHeaderData(7, Qt::Horizontal, QString("t РІРґРѕС…"));
    examsModel->setHeaderData(8, Qt::Horizontal, QString("t РІС‹РґРѕС…"));
    examsModel->setHeaderData(9, Qt::Horizontal, QString("РћР±СЉС‘Рј"));
    ui->examsView->setModel(examsModel);
    ui->examsView->setColumnHidden(0,true);
    ui->examsView->setColumnHidden(1,true);
    ui->examsView->setColumnHidden(3,true);
    ui->examsView->setColumnHidden(5,true);
    ui->examsView->setColumnHidden(7,true);
    ui->examsView->setColumnHidden(8,true);
    ui->examsView->setColumnHidden(9,true);
    ui->examsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->examsView->setEditTriggers(QTableView::NoEditTriggers);

}
