#include "TSPatients.h"

TSPatients::TSPatients(QSqlDatabase connection) :
    QSqlTableModel(0,connection)
{
    // Setting up models and table views
    setTable("patients");
    setEditStrategy(QSqlTableModel::OnRowChange);
    select();
    setHeaderData(1, Qt::Horizontal, tr("ID ��������"));
    setHeaderData(2, Qt::Horizontal, tr("�������"));
    setHeaderData(3, Qt::Horizontal,tr("���"));
    setHeaderData(4, Qt::Horizontal, tr("��������"));
    setHeaderData(7, Qt::Horizontal, tr("���� ��������"));
}
