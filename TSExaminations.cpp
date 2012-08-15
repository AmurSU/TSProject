#include "TSExaminations.h"

TSExaminations::TSExaminations(QSqlDatabase connection) :
    QSqlTableModel()
{
    examsModel = new QSqlTableModel(this, connection);
    examsModel->setTable("examinations");
    examsModel->setEditStrategy(QSqlTableModel::OnRowChange);
    examsModel->select();
    //qDebug()<<"Filter payments by account "<<examsModel->filter();
    examsModel->setHeaderData(0, Qt::Horizontal, QString("ID"));
    examsModel->setHeaderData(1, Qt::Horizontal, QString("Код"));
    examsModel->setHeaderData(2, Qt::Horizontal, QString("Дата"));
    examsModel->setHeaderData(3, Qt::Horizontal, QString("Показания"));
    examsModel->setHeaderData(4, Qt::Horizontal, QString("Диагноз"));
    examsModel->setHeaderData(5, Qt::Horizontal, QString("Медсестра"));
    examsModel->setHeaderData(6, Qt::Horizontal, QString("Врач"));
    examsModel->setHeaderData(7, Qt::Horizontal, QString("t вдох"));
    examsModel->setHeaderData(8, Qt::Horizontal, QString("t выдох"));
    examsModel->setHeaderData(9, Qt::Horizontal, QString("Объём"));
}
