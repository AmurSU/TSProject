#ifndef TSEXAMINATIONS_H
#define TSEXAMINATIONS_H

#include <QSqlTableModel>

class TSExaminations : public QSqlTableModel
{
public:
    explicit TSExaminations(QSqlDatabase connection);
};

#endif // TSEXAMINATIONS_H
