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
        msg.setText("Не удалось установить соединение с базой данных.Проверьте права доступа, парасетры соединения");
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
    patientsModel->setHeaderData(1, Qt::Horizontal, QString("Код"));
    patientsModel->setHeaderData(2, Qt::Horizontal, QString("Фамилия"));
    patientsModel->setHeaderData(3, Qt::Horizontal, QString("Имя"));
    patientsModel->setHeaderData(4, Qt::Horizontal, QString("Год рождения"));
    patientsModel->setHeaderData(5, Qt::Horizontal, QString("Рост"));
    patientsModel->setHeaderData(6, Qt::Horizontal, QString("Вес"));
    patientsModel->setHeaderData(7, Qt::Horizontal, QString("Пол"));
    patientsModel->setHeaderData(8, Qt::Horizontal, QString("Курит?"));
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
    examsModel->setHeaderData(1, Qt::Horizontal, QString("Код"));
    examsModel->setHeaderData(2, Qt::Horizontal, QString("Дата"));
    examsModel->setHeaderData(3, Qt::Horizontal, QString("Показания"));
    examsModel->setHeaderData(4, Qt::Horizontal, QString("Диагноз"));
    examsModel->setHeaderData(5, Qt::Horizontal, QString("Медсестра"));
    examsModel->setHeaderData(6, Qt::Horizontal, QString("Врач"));
    examsModel->setHeaderData(7, Qt::Horizontal, QString("T вдох"));
    examsModel->setHeaderData(8, Qt::Horizontal, QString("T выдох"));
    examsModel->setHeaderData(9, Qt::Horizontal, QString("Объём"));
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
