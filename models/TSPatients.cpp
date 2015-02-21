#include "TSPatients.h"

TSPatients::TSPatients(QSqlDatabase connection) :
    QSqlTableModel(0,connection)
{
    // Setting up models and table views
    setTable("patients");
    setEditStrategy(QSqlTableModel::OnRowChange);
    select();
    /*setHeaderData(1, Qt::Horizontal, "ID ��������");
    setHeaderData(2, Qt::Horizontal, "�������");
    setHeaderData(3, Qt::Horizontal,"���");
    setHeaderData(4, Qt::Horizontal, "��������");
    setHeaderData(7, Qt::Horizontal, "���� ��������");*/
    setHeaderData(1, Qt::Horizontal, "ID");
    setHeaderData(2, Qt::Horizontal, "Second name");
    setHeaderData(3, Qt::Horizontal,"First name");
    setHeaderData(4, Qt::Horizontal, "Patronymic");
    setHeaderData(7, Qt::Horizontal, "Date of birth");
}
