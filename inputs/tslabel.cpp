#include "tslabel.h"
#include <QPoint>

TSLabel::TSLabel(QWidget *parent) :
    QLabel(parent)
{
}

void TSLabel::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint pos = ev->pos();
    emit mousePositionChanged(pos);
}
