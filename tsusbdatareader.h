#ifndef TSUSBDATAREADER_H
#define TSUSBDATAREADER_H

#include <QObject>
#include "QLibrary"
#include "Rtusbapi.h"
#include "QDebug"
#include "tscurvebuffer.h"
#include "extremum.h"
class TSUsbDataReader : public QObject
{
    Q_OBJECT
public:
    explicit TSUsbDataReader(QObject *parent = 0);
    ~TSUsbDataReader();
    bool initDevice();
    void stopRead();
    void setReadingType(TSUsbReadingType type);
    void setBuffer(TSCurveBuffer *bffr);
    bool isReady();
signals:
    //void done(bool isCorrect);
    void done();
    void changeProgress(int val);
public slots:
    void doWork();
private:
    bool read();
    void releaseReader();
    TSCurveBuffer *buffer;
    SHORT* readData();
    DWORD DllVersion;
    IRTUSB3000 *pModule; // ��������� �� ��������� ������
    const static WORD CHANNELS_QUANTITY = 0x4; // ���-�� ������������ ������� ������
    SHORT AdcBuffer[CHANNELS_QUANTITY]; // ����� ������ ��� ����� ��������
    TSUsbReadingType readingType;
    bool ReadingStarted;
};

#endif // TSUSBDATAREADER_H
