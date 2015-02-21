#include "TSPatients.h"

TSPatients::TSPatients(QSqlDatabase connection) :
    QSqlTableModel(0,connection)
{
    // Setting up models and table views
    setTable("patients");
    setEditStrategy(QSqlTableModel::OnRowChange);
    select();
    /*setHeaderData(1, Qt::Horizontal, "ID пациента");
    setHeaderData(2, Qt::Horizontal, "Фамилия");
    setHeaderData(3, Qt::Horizontal,"Имя");
    setHeaderData(4, Qt::Horizontal, "Отчество");
    setHeaderData(7, Qt::Horizontal, "Дата рождения");*/
    setHeaderData(1, Qt::Horizontal, "ID");
    setHeaderData(2, Qt::Horizontal, "Second name");
    setHeaderData(3, Qt::Horizontal,"First name");
    setHeaderData(4, Qt::Horizontal, "Patronymic");
    setHeaderData(7, Qt::Horizontal, "Date of birth");
}
