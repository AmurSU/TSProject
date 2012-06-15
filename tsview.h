#ifndef TSVIEW_H
#define TSVIEW_H

#include <QMainWindow>
#include "tscontroller.h"
#include "tsiview.h"
#include <QPen>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

namespace Ui {
    class TSView;
}

class TSView : public QMainWindow,public TSIView
{
    Q_OBJECT
public:
    explicit TSView(QWidget *parent = 0);
    ~TSView();
    void showGUI();
    void setController(TSController *c);
private:
    Ui::TSView *ui;
    TSController *control;
    // Все для рисования
    QPixmap volPixmap;
    QPixmap tinPixmap;
    QPixmap toutPixmap;
    QPainter vol;
    QPainter tin;
    QPainter tout;
    QTimer *paintTimer;
};

#endif // TSVIEW_H
