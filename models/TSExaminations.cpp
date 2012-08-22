#include "TSExaminations.h"

TSExaminations::TSExaminations(QSqlDatabase connection) :
    QSqlTableModel(0,connection)
{

    setTable("examinations");
    setEditStrategy(QSqlTableModel::OnRowChange);
    select();
    //qDebug()<<"Filter payments by account "<< filter();
     setHeaderData(0, Qt::Horizontal, QString("ID"));
     setHeaderData(1, Qt::Horizontal, tr("Дата"));
     setHeaderData(2, Qt::Horizontal, tr("Показания"));
     setHeaderData(3, Qt::Horizontal, tr("Диагноз"));
     setHeaderData(4, Qt::Horizontal, tr("Медсестры"));
     setHeaderData(5, Qt::Horizontal, tr("Врач"));
     setHeaderData(6, Qt::Horizontal, tr("Tin"));
     setHeaderData(7, Qt::Horizontal, tr("Tout"));
     setHeaderData(8, Qt::Horizontal, tr("Объём"));
}
