#ifndef TSEXAMINATIONS_H
#define TSEXAMINATIONS_H

#include <QtSql>

class TSExaminations : public QSqlTableModel
{
public:
    TSExaminations(QSqlDatabase connection);
    QSqlTableModel *examsModel;
};

#endif // TSEXAMINATIONS_H
