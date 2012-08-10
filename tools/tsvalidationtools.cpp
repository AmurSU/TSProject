#include "tsvalidationtools.h"

TSValidationTools::TSValidationTools()
{

}

QVariant TSValidationTools::isInt(QString value)
{
    QRegExp regExp("\\d+");
    if(regExp.exactMatch(value))
    {
        return QVariant(value);
    }
    else
    {
        return QVariant(false);
    }
}

QVariant TSValidationTools::isNameString(QString value)
{
    QRegExp regExp(QString::fromLocal8Bit("[À-ß]+"));
    value = value.toUpper();
    if(regExp.exactMatch(value))
    {
        return QVariant(value);
    }
    else
    {
        return QVariant(false);
    }
}
