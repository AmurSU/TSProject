#ifndef TSLINEEDIT_H
#define TSLINEEDIT_H

#include <QLineEdit>
#include <QWidget>

class TSLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit TSLineEdit(QWidget *parent = 0);
    void bindWithString(QString* string);

signals:

public slots:
    void refreshBindedString(QString string);
private:
    QString *bind;

};

#endif // TSLINEEDIT_H
