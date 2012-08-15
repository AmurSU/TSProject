#include "TSPatients.h"

TSPatients::TSPatients(QSqlDatabase connection) :
    QSqlTableModel()
{
    // Setting up models and table views
    patientsModel = new QSqlTableModel(this, connection);
    patientsModel->setTable("patients");
    patientsModel->setEditStrategy(QSqlTableModel::OnRowChange);
    patientsModel->select();
    patientsModel->setHeaderData(0, Qt::Horizontal, QString("ID"));
    patientsModel->setHeaderData(1, Qt::Horizontal, QString("Код"));
    patientsModel->setHeaderData(2, Qt::Horizontal, QString("Фамилия"));
    patientsModel->setHeaderData(3, Qt::Horizontal, QString("Имя"));
    patientsModel->setHeaderData(4, Qt::Horizontal, QString("Рождение"));
    patientsModel->setHeaderData(5, Qt::Horizontal, QString("Рост"));
    patientsModel->setHeaderData(6, Qt::Horizontal, QString("Вес"));
    patientsModel->setHeaderData(7, Qt::Horizontal, QString("Пол"));
    patientsModel->setHeaderData(8, Qt::Horizontal, QString("Курит?"));
}
