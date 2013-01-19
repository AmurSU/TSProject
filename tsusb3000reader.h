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
    IRTUSB3000 *pModule; // указатель на интерфейс модуля
    char ModuleName[10]; // название модуля
    BYTE UsbSpeed; // скорость работы шины USB
    char ModuleSerialNumber[9]; // серийный номер модуля
    char AvrVersion[5]; // версия драйвера AVR
    RTUSB3000::DSP_INFO di; // структура, содержащая информацию о версии драйвера DSP
    RTUSB3000::FLASH fi; // структура информации в ППЗУ модуля
    RTUSB3000::INPUT_PARS ip; // структура параметров работы АЦП
    const static WORD CHANNELS_QUANTITY = 0x4; // кол-во опрашиваемых каналов модуля
    const static double ReadRate = 1.0; // частота  ввода данных
    const static WORD MaxVirtualSoltsQuantity = 4;
    SHORT AdcBuffer[CHANNELS_QUANTITY]; // буфер данных для кадра отсчётов


    // номер ошибки при выполнении потока сбора данных
    WORD ThreadErrorNumber;
    // флажок завершения потоков ввода данных
    bool IsThreadComplete;
    //max возможное кол-во передаваемых отсчетов (кратное 32) для ф. ReadData и WriteData()
    DWORD DataStep;
    // столько блоков по DataStep отсчётов нужно собрать в файл
    WORD NBlockRead;
    // указатель на буфер для вводимых данных
    SHORT	*ReadBuffer;
    DWORD Counter;
    // хэндл модуля
    HANDLE ModuleHandle;
    // номер ошибки при выполнении потока сбора данных
    bool WaitingForRequestCompleted(OVERLAPPED *ReadOv);
signals:
    void done();
    void changeProgress(int val);
public slots:

};

#endif // TSUSB3000READER_H
