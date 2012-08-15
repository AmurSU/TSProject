#include "tsdbupdate.h"


TSDBupdate::TSDBupdate(QObject *parent, QSqlDatabase connection, QString table)
{
    Table = table;
}


void TSDBupdate::execute(QSqlRecord oldRecord, QSqlRecord newRecord)
{

    QSqlQuery q;
    q.prepare("SELECT * FROM :table");
    q.bindValue(":table",Table);
    q.exec();


}
