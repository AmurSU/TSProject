#include "tscombobox.h"

TSComboBox::TSComboBox(QWidget *parent) :
    QComboBox(parent)
{
    bind = 0;
}

void TSComboBox::bindWithString(QString *string)
{
    bind = string;
    connect(this,SIGNAL(currentIndexChanged(QString)),this,SLOT(refreshBindedString(QString)));
    *bind = currentText();
}

void TSComboBox::refreshBindedString(QString string)
{
    if(bind ==0) return;
    *bind = string;
}
