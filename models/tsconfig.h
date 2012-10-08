#ifndef TSCONFIG_H
#define TSCONFIG_H

#include <QDomDocument>
#include <QDomElement>
#include <QFile>

class TSConfig : public QDomDocument
{
public:
    TSConfig();
    TSConfig(QString confName);
    int setConfName(QString confName);
    QString confName();
    QString errorToString();

private:
    QString ts_confName;
    QString ts_xmlError;
    int eRow;
    int eColumn;
    QFile* ts_confFile;

};

#endif // TSCONFIG_H
