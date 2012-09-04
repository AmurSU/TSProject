#ifndef TSANALITICS_H
#define TSANALITICS_H

#include <QObject>
#include <QVector>
struct extremum {
    int x, y, type;
};

class tsanalitics : public QObject
{
    Q_OBJECT
public:
    explicit tsanalitics(QObject *parent = 0);
    QVector<int> getMovingAverages(int Period);
    float getMaxAvgs();
    float getdMinAvgs();
    float getFrequency();
    int getMaxsCount();
    int getMinsCount();
    int getMinMaxAvgTime();
    int getMaxMinAvgTime();
    int getTime();
    int getPeriodTime();
    int setupData(QVector<int> *row_d);
    int findExtremums();
    int deleteBadExtremums();
private:
    int getMax();
    int getMin();
    QVector<extremum> *ts_extremums;
    QVector<int> *ts_row_data;
signals:
    
public slots:
    
};

#endif // TSANALITICS_H
