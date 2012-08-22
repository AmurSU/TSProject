#include "TSPatients.h"

TSPatients::TSPatients(QSqlDatabase connection) :
    QSqlTableModel(0,connection)
{
    // Setting up models and table views
    setTable("patients");
    setEditStrategy(QSqlTableModel::OnRowChange);
    select();
    setHeaderData(1, Qt::Horizontal, tr("ID пациента"));
    setHeaderData(2, Qt::Horizontal, tr("Фамилия"));
    setHeaderData(3, Qt::Horizontal,tr("Имя"));
    setHeaderData(4, Qt::Horizontal, tr("Отчество"));
    setHeaderData(7, Qt::Horizontal, tr("Дата рождения"));
}
