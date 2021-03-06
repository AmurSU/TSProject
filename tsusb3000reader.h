#ifndef TSUSB3000READER_H
#define TSUSB3000READER_H

#include <QObject>

#include <QtCore/QThread>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "QLibrary"
#include "Rtusbapi.h"
#include "QDebug"
#include "tscurvebuffer.h"
class TSUsb3000Reader : public QObject
{
    Q_OBJECT
public:
    explicit TSUsb3000Reader(QObject *parent = 0);
    int calibtateVolume();
    SHORT* readData();
    bool initDevice(TSCurveBuffer *_bf);
    QString getLastError();
    void TerminateApplication(char *ErrorString, bool TerminationFlag = false);
    bool closeReader();
    ~TSUsb3000Reader();
    int getQuietLevels(int *qlevels, int qtime);
    DWORD read();
    void setLastError(QString);
    void run();
private:
    QString LastError;
    TSCurveBuffer *buffer;
    DWORD DllVersion;
    IRTUSB3000 *pModule; // ��������� �� ��������� ������
    char ModuleName[10]; // �������� ������
    BYTE UsbSpeed; // �������� ������ ���� USB
    char ModuleSerialNumber[9]; // �������� ����� ������
    char AvrVersion[5]; // ������ �������� AVR
    RTUSB3000::DSP_INFO di; // ���������, ���������� ���������� � ������ �������� DSP
    RTUSB3000::FLASH fi; // ��������� ���������� � ���� ������
    RTUSB3000::INPUT_PARS ip; // ��������� ���������� ������ ���
    const static WORD CHANNELS_QUANTITY = 0x4; // ���-�� ������������ ������� ������
    const static double ReadRate = 1.0; // �������  ����� ������
    const static WORD MaxVirtualSoltsQuantity = 4;
    SHORT AdcBuffer[CHANNELS_QUANTITY]; // ����� ������ ��� ����� ��������


    // ����� ������ ��� ���������� ������ ����� ������
    WORD ThreadErrorNumber;
    // ������ ���������� ������� ����� ������
    bool IsThreadComplete;
    //max ��������� ���-�� ������������ �������� (������� 32) ��� �. ReadData � WriteData()
    DWORD DataStep;
    // ������� ������ �� DataStep �������� ����� ������� � ����
    WORD NBlockRead;
    // ��������� �� ����� ��� �������� ������
    SHORT	*ReadBuffer;
    DWORD Counter;
    // ����� ������
    HANDLE ModuleHandle;
    // ����� ������ ��� ���������� ������ ����� ������
    bool WaitingForRequestCompleted(OVERLAPPED *ReadOv);
signals:
    void done();
    void changeProgress(int val);
public slots:

};

#endif // TSUSB3000READER_H
