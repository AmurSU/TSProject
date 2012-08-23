#ifndef CRUDMW_H
#define CRUDMW_H

//#include <QtCore>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QDebug>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QSqlDatabase dbConnect();
    QSqlTableModel *patientsModel, *examsModel;
    QModelIndex indexx;
protected slots:
    void on_newExamButton_clicked();
    void on_rowOfPTDoubleClicked(QModelIndex ind);

    //void on_StringFT_picked();
    /*on_searchButton_clicked();
    on_moreButton_clicked();
    on_newPatientButton_clicked();
    on_printButton_clicked();
    on_viewButton_clicked();
    on_deleteButton_clicked();
    on_editButton_clicked();
    on_StringFT_picked();
    on_StringST_picked();*/

};

#endif // CRUDMW_H
