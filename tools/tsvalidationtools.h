#ifndef TSVALIDATIONTOOLS_H
#define TSVALIDATIONTOOLS_H

#include <QRegExp>
#include <QVariant>

class TSValidationTools
{
public:
    TSValidationTools();
    static QVariant isInt(QString value);
    static QVariant isNameString(QString value);
private:
};

#endif // TSVALIDATIONTOOLS_H
