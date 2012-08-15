#ifndef TSPATIENTS_H
#define TSPATIENTS_H

#include <QtSql>

class TSPatients : public QSqlTableModel
{
public:
    TSPatients(QSqlDatabase connection);
    QSqlTableModel *patientsModel;
signals:
    
public slots:
    
};

#endif // TSPATIENTS_H
