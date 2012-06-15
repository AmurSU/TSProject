#ifndef TSCOMBOBOX_H
#define TSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>

class TSComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TSComboBox(QWidget *parent = 0);
    void bindWithString(QString* string);
signals:

public slots:
    void refreshBindedString(QString string);
private:
    QString *bind;
};

#endif // TSCOMBOBOX_H
