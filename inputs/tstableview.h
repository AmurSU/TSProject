#ifndef TSTABLEVIEW_H
#define TSTABLEVIEW_H

#include <QTableView>

class TSTableView : public QTableView
{
    Q_OBJECT
public:
    explicit TSTableView(QWidget *parent = 0);
protected:
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void deleteRequest(int row);
public slots:

};

#endif // TSTABLEVIEW_H
