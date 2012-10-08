#include "tsconfig.h"
#include <QDebug>

TSConfig::TSConfig()
{
    ;
}

TSConfig::TSConfig(QString confName)
{
    setConfName(confName);
}

int TSConfig::setConfName(QString confName)
{
    ts_confName = confName;
    ts_confFile = new QFile("ts_config.txt");
    if(!ts_confFile->exists())
    {
        qDebug()<<"Config file is not found";
        return 1;
    }
    if(!setContent(ts_confFile,&ts_xmlError,&eRow,&eColumn))
    {
            errorToString();
            return 2;
    }
    return 0;
}

QString TSConfig::errorToString()
{
    return "Error: "+ts_xmlError+" at row "+QString::number(eRow)+
            " column "+QString::number(eColumn);
}

QString TSConfig::confName()
{
    return ts_confName;
}
