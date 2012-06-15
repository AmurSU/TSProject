#ifndef TSREADERTHREAD_H
#define TSREADERTHREAD_H

#include <QThread>

class TSReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit TSReaderThread(QObject *parent = 0);

signals:

public slots:

};

#endif // TSREADERTHREAD_H
