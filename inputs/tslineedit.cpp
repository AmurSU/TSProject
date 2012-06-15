#include "tslineedit.h"

TSLineEdit::TSLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    bind = 0;
}

void TSLineEdit::bindWithString(QString *string)
{
    bind = string;
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(refreshBindedString(QString)));
    *bind = text();
}

void TSLineEdit::refreshBindedString(QString string)
{
    if(bind==0)return;
    *bind = string;
}
