#ifndef TSLABEL_H
#define TSLABEL_H

#include <QLabel>
#include <QMouseEvent>

class TSLabel : public QLabel
{
    Q_OBJECT
public:
    explicit TSLabel(QWidget *parent = 0);
protected:
    void mouseMoveEvent(QMouseEvent *ev);
signals:
    void mousePositionChanged(QPoint p);
public slots:

};

#endif // TSLABEL_H
