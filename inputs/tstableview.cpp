#include "tstableview.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTranslator>

TSTableView::TSTableView(QWidget *parent) :
    QTableView(parent)
{
}

void TSTableView::keyReleaseEvent(QKeyEvent *event){

    if(event->key()==Qt::Key_Delete){
        QMessageBox mess((QWidget*)parent());
        mess.setWindowTitle(tr("Вопрос"));
        mess.setText(tr("Вы уверены, что хотите удалить запись."));
        mess.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        if(mess.exec()==QMessageBox::Ok){
            int ind = selectionModel()->selectedRows().at(0).row();
            emit deleteRequest(ind);
        }
    }
}
