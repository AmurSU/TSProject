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
    int getMaxAvgs();
    int getMinAvgs();
    int getFrequency();
    float getAvgExpirationFlowRate();
    int getMaxsCount();
    int getMinsCount();
    int getMinMaxAvgTime();
    int getMaxMinAvgTime();
    int getTime();
    int getAvgInspiratoryTime();
    int getAvgExpiratoryTime();
    int getPeriodTime();
    int setupData(QVector<int> *row_d);
    int findExtremums();
    int deleteBadExtremums();
    int getBreathingVolume();
    int getAvgInspiratorySpeed();
    int getAvgExpiratorySpeed();
    int getMaxInspiratorySpeed();
    int getMaxExpiratorySpeed();
    void append(int n);
    void clear();
    int getMaxsSum();
    int getMinsSum();
    int getMVL();
    void printVec(int k);
    int getMax();
    int getMin();
    QVector<extremum>* getExtremums();
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

#endif // TSANALITICS_H
