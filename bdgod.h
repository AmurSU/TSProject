#ifndef BDGOD_H
#define BDGOD_H
#include <QtSql>

namespace BDG {class BDgod;}

class BDgod
{
    //Q_OBJECT
public:
    BDgod();
    ~BDgod();
    QSqlDatabase dbConnect(QString);
    static void CreateNewDatabase(QString ID,QString name, QString surname);
    void CacheInd(QModelIndex ind);
    QModelIndex GetCachedInd();
    static QString Transliterate(QString str);
    QModelIndex index;
private:

};

#endif // BDGOD_H
