#include "bdgod.h"
#include <QMessageBox>

BDgod::BDgod(){
}

BDgod::~BDgod()
{

}


QSqlDatabase BDgod::dbConnect(QString baseName)
{
    // Connecting to database

    QSqlDatabase baseConnector = QSqlDatabase::addDatabase("QSQLITE");
    baseConnector.setDatabaseName(baseName);
    if(baseConnector.open()) {
        qDebug()<<"Connected to database";
    } else {
        QMessageBox msg;
        msg.setText("Не удалось подсоединится к базе данных. Проверьте права доступа.");
        msg.show();
        qDebug()<<"Not connected to database";
       }
    return baseConnector;

}

void BDgod::CreateNewDatabase(QString ID,QString name, QString surname)
{
    QString str;
    str = ID + "_" + name + "_" + surname;
    qDebug()<<str;
    QSqlDatabase base = QSqlDatabase::addDatabase("QSQLITE","newConnection");
    base.setDatabaseName(BDgod::Transliterate(str));
    if (!base.open())
        qDebug()<<"Failed to create database";
    QSqlQuery q;
    if (!q.exec("create table DirInfo (id int primary key,"
                 "idDir int,"
                 "tag text(30),"
                 "Value text(20),"))
        qDebug()<<"Query failed";
    base.close();
}

void BDgod::CacheInd(QModelIndex ind)
{
    index = ind;
}

QModelIndex BDgod::GetCachedInd()
{
    return index;
}

QString BDgod::Transliterate(QString str)
{
    QString fn;
    int i, rU, rL;
    QString validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-_,.()[]{}<>~!@#$%^&*+=?";
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QString rusUpper = QObject::tr("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЫЭЮЯ");
    QString rusLower = QObject::tr("абвгдеёжзийклмнопрстуфхцчшщыэюя");
    QStringList latUpper, latLower;
    latUpper <<"A"<<"B"<<"V"<<"G"<<"D"<<"E"<<"Jo"<<"Zh"<<"Z"<<"I"<<"Y"<<"K"<<"L"<<"M"<<"N"
        <<"O"<<"P"<<"R"<<"S"<<"T"<<"U"<<"F"<<"H"<<"C"<<"Ch"<<"Sh"<<"Sh"<<"I"<<"E"<<"Ju"<<"Ja";
    latLower <<"a"<<"b"<<"v"<<"g"<<"d"<<"e"<<"jo"<<"zh"<<"z"<<"i"<<"y"<<"k"<<"l"<<"m"<<"n"
        <<"o"<<"p"<<"r"<<"s"<<"t"<<"u"<<"f"<<"h"<<"c"<<"ch"<<"sh"<<"sh"<<"i"<<"e"<<"ju"<<"ja";
    for (i=0; i < str.size(); ++i){
        if ( validChars.contains(str[i]) ){
            fn = fn + str[i];
        }else if (str[i] == ' '){  //replace spaces
            fn = fn + "_";
        }else{
            rU = rusUpper.indexOf(str[i]);
            rL = rusLower.indexOf(str[i]);
            if (rU > 0)         fn = fn + latUpper[rU];
            else if (rL > 0)   fn = fn + latLower[rL];
        }
    }
    if (fn.isEmpty() ) fn = "file";
    return fn;
}

