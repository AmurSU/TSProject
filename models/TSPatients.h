#ifndef TSPATIENTS_H
#define TSPATIENTS_H

#include <QSqlTableModel>

class TSPatients : public QSqlTableModel
{
public:
    explicit TSPatients(QSqlDatabase connection);
signals:
    
public slots:
    
};

#endif // TSPATIENTS_H
