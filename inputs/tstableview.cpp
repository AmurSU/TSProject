#include "tstableview.h"
#include <QKeyEvent>
#include <QDebug>

TSTableView::TSTableView(QWidget *parent) :
    QTableView(parent)
{
}

void TSTableView::keyReleaseEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Delete){
        int ind = selectionModel()->selectedRows().at(0).row();
        emit deleteRequest(ind);
    }
}
