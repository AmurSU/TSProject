#ifndef TSTEMPANALITIC_H
#define TSTEMPANALITIC_H

#include <QObject>
#include <extremum.h>
class tstempanalitic : public QObject
{
    Q_OBJECT
public:
    explicit tstempanalitic(QObject *parent = 0);
    ~tstempanalitic();
    int setupData(QVector<int> *row_d);
    void append(int n);
    int findExtremums();
    int deleteBadExtremums();
    int deleteBadExtremumsVolume();
    int getMaxAvgs();
    int getMinAvgs();
    int getMaxsCount();
    int getMinsCount();
    int getMinMaxAvgTime();
    int getMaxMinAvgTime();
    void clear();
    int getMaxsSum();
    int getMinsSum();
    int getMax();
    int getMin();
private:
    int fabs(int a);
    void deleteEqualSignExtremums();
    void deleteSimilarInMeaningExtremums();
    void deletePatternLightningExtremums();
    QVector<extremum> *ts_extremums;
    QVector<int> *ts_row_data;
signals:
    
public slots:
    
};

#endif // TSTEMPANALITIC_H
