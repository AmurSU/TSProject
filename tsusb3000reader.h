#ifndef TSUSB3000READER_H
#define TSUSB3000READER_H

#include <QObject>

class TSUsb3000Reader : public QObject
{
    Q_OBJECT
public:
    explicit TSUsb3000Reader(QObject *parent = 0);

signals:

public slots:

};

#endif // TSUSB3000READER_H
