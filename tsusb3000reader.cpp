#include "tsusb3000reader.h"
#include <windows.h>
#include <QTimer>

TSUsb3000Reader::TSUsb3000Reader(QObject *parent) :
    QObject(parent)
{
    //IsThreadComplete=false;
    DataStep = 1024*1024;
    NBlockRead = 20;
    DWORD Counter = 0x0;
}
bool TSUsb3000Reader::initDevice(TSCurveBuffer *_bf){
    buffer=_bf;
    typedef DWORD(*GetDllVersion)();
    typedef LPVOID(*CreateInstance)(PCHAR const);
    QLibrary myLib("dll/Rtusbapi.dll");
    myLib.load();

    GetDllVersion RtGetDllVersion = (GetDllVersion) myLib.resolve("RtGetDllVersion");
    if (RtGetDllVersion) {
        if (RtGetDllVersion() == CURRENT_VERSION_RTUSBAPI){
            qDebug() << "Dll version is correct";
        }
        else{
            qDebug() << "Dll version isn`t correct";
            this->setLastError("Dll version isn`t correct");
            return false;
        }
    }

    CreateInstance RtCreateInstance = (CreateInstance) myLib.resolve("RtCreateInstance");
    if (RtCreateInstance) {
        pModule = static_cast<IRTUSB3000 *> (RtCreateInstance("usb3000"));
        if(pModule == NULL){
            qDebug() << "Can`t create usb3000 instance";
            return false;
        }else{
            qDebug() << "Create usb3000 instance";

        }
    }
    WORD i;
    // проверим версию используемой библиотеки Rtusbapi.dll
    if ((DllVersion = RtGetDllVersion()) != CURRENT_VERSION_RTUSBAPI) {
        char String[128];
        sprintf(String, " Rtusbapi.dll Version Error!!!\n   Current: %1u.%1u. Required: %1u.%1u",
                DllVersion >> 0x10, DllVersion & 0xFFFF,
                CURRENT_VERSION_RTUSBAPI >> 0x10, CURRENT_VERSION_RTUSBAPI & 0xFFFF);

        TerminateApplication(String);
    } else printf(" Rtusbapi.dll Version --> OK\n");

    // получим указатель на интерфейс модуля USB3000
    pModule = static_cast<IRTUSB3000 *> (RtCreateInstance("usb3000"));
    if (!pModule) TerminateApplication(" Module Interface --> Bad\n");
    else printf(" Module Interface --> OK\n");

    // попробуем обнаружить модуль USB3000 в первых 127 виртуальных слотах
    for (i = 0x0; i < MaxVirtualSoltsQuantity; i++) if (pModule->OpenDevice(i)) break;
    // что-нибудь обнаружили?
    if (i == MaxVirtualSoltsQuantity) TerminateApplication(" Can't find module USB3000 in first 127 virtual slots!\n");
    else printf(" OpenDevice(%u) --> OK\n", i);

    // прочитаем название обнаруженного модуля
    if (!pModule->GetModuleName(ModuleName)) TerminateApplication(" GetModuleName() --> Bad\n");
    else printf(" GetModuleName() --> OK\n");

    // проверим, что это 'USB3000'
    if (strcmp(ModuleName, "USB3000")) TerminateApplication(" The module is not 'USB3000'\n");
    else printf(" The module is 'USB3000'\n");

    // узнаем текущую скорость работы шины USB20
    if (!pModule->GetUsbSpeed(&UsbSpeed)) {
        printf(" GetUsbSpeed() --> Bad\n");
        exit(1);
    } else printf(" GetUsbSpeed() --> OK\n");
    // теперь отобразим версию драйвера AVR
    printf(" USB Speed is %s\n", UsbSpeed ? "HIGH (480 Mbit/s)" : "FULL (12 Mbit/s)");

    // прочитаем серийный номер модуля
    if (!pModule->GetModuleSerialNumber(ModuleSerialNumber)) TerminateApplication(" GetModuleSerialNumber() --> Bad\n");
    else printf(" GetModuleSerialNumber() --> OK\n");
    // теперь отобразим серийный номер модуля
    printf(" Module Serial Number is %s\n", ModuleSerialNumber);

    // прочитаем версию драйвера AVR
    if (!pModule->GetAvrVersion(AvrVersion)) TerminateApplication(" GetAvrVersion() --> Bad\n");
    else printf(" GetAvrVersion() --> OK\n");
    // теперь отобразим версию драйвера AVR
    printf(" Avr Driver Version is %s\n", AvrVersion);

    // код драйвера DSP возьмём из соответствующего ресурса штатной DLL библиотеки
    if (!pModule->LOAD_DSP()) TerminateApplication(" LOAD_DSP() --> Bad\n");
    else printf(" LOAD_DSP() --> OK\n");

    // проверим загрузку модуля
    if (!pModule->MODULE_TEST()) TerminateApplication(" MODULE_TEST() --> Bad\n");
    else printf(" MODULE_TEST() --> OK\n");

    // получим версию загруженного драйвера DSP
    if (!pModule->GET_DSP_INFO(&di)) TerminateApplication(" GET_DSP_INFO() --> Bad\n");
    else printf(" GET_DSP_INFO() --> OK\n");
    // теперь отобразим версию загруженного драйвера DSP
    printf(" DSP Driver version is %1u.%1u\n", di.DspMajor, di.DspMinor);

    // обязательно проинициализируем поле size структуры RTUSB3000::FLASH
    fi.size = sizeof (RTUSB3000::FLASH);
    // получим информацию из ППЗУ модуля
    if (!pModule->GET_FLASH(&fi)) TerminateApplication(" GET_FLASH() --> Bad\n");
    else printf(" GET_FLASH() --> OK\n");

    // обязательно проинициализируем поле size структуры RTUSB3000::INPUT_PARS
    ip.size = sizeof (RTUSB3000::INPUT_PARS);
    // получим текущие параметры работы АЦП
    if (!pModule->GET_INPUT_PARS(&ip)) TerminateApplication(" GET_INPUT_PARS() --> Bad\n");
    else printf(" GET_INPUT_PARS() --> OK\n");

    // установим желаемые параметры ввода данных
    ip.CorrectionEnabled = true; // разрешим корректировку данных
    ip.InputClockSource = RTUSB3000::INTERNAL_INPUT_CLOCK; // будем использовать внутренние тактовые испульсы для ввода данных
    //	ip.InputClockSource = RTUSB3000::EXTERNAL_INPUT_CLOCK;	// будем использовать внешние тактовые испульсы для ввода данных
    ip.SynchroType = RTUSB3000::NO_SYNCHRO; // не будем использовать никакую синхронизацию при вводе данных
    //	ip.SynchroType = RTUSB3000::TTL_START_SYNCHRO;	// будем использовать цифровую синхронизацию старта при вводе данных
    ip.ChannelsQuantity = CHANNELS_QUANTITY; // четыре активных канала
    for (i = 0x0; i < CHANNELS_QUANTITY; i++) ip.ControlTable[i] = (WORD) (i);
    ip.InputRate = ReadRate; // частота работы АЦП в кГц
    ip.InterKadrDelay = 0.0;
    // будем использовать фирменные калибровочные коэффициенты, которые храняться в ППЗУ модуля
    for (i = 0x0; i < 8; i++) {
        ip.AdcOffsetCoef[i] = fi.AdcOffsetCoef[i];
        ip.AdcScaleCoef[i] = fi.AdcScaleCoef[i];
    }
    // передадим требуемые параметры работы ввода данных в драйвер DSP модуля
    if (!pModule->SET_INPUT_PARS(&ip)) TerminateApplication(" SET_INPUT_PARS() --> Bad\n");
    else printf(" SET_INPUT_PARS() --> OK\n");

    // отобразим параметры модуля на экране монитора
    printf(" \n");
    printf(" Module USB3000 (S/N %s) is ready ... \n", fi.SerialNumber);
    printf(" Adc parameters:\n");
    printf("   InputClockSource is %s\n", ip.InputClockSource ? "EXTERNAL" : "INTERNAL");
    printf("   SynchroType is %s\n", ip.SynchroType ? "TTL_START_SYNCHRO" : "NO_SYNCHRO");
    printf("   ChannelsQuantity = %2d\n", ip.ChannelsQuantity);
    printf("   AdcRate = %8.3f kHz\n", ip.InputRate);
    //printf("   InterKadrDelay = %2.4f ms\n", ip.InterKadrDelay);
    printf("   ChannelRate = %8.3f kHz\n", ip.ChannelRate);
    printf("\n Press any key to terminate this program...\n");
    // цикл перманентного выполнения функции ADC_KADR и
    // отображения полученных данных на экране диплея

    //emit сигнал о том что поток создан
}
SHORT* TSUsb3000Reader::readData(){
    if (pModule->READ_KADR(AdcBuffer)) {
        //buffer->append(AdcBuffer[0],AdcBuffer[1],AdcBuffer[2]);
        return AdcBuffer;
    }
    else
    {
        this->setLastError("Can`t read from device");
        return NULL;
    }
}
bool TSUsb3000Reader::closeReader(){
    qDebug()<<"close Reader";
    if (pModule->CloseDevice()) {
        return true;
    }
    else
    {
        this->setLastError("Can`t close device");
        return false;
    }
}

void TSUsb3000Reader::TerminateApplication(char *ErrorString, bool TerminationFlag) {
    // подчищаем интерфейс модуля
    if (pModule) {
        // освободим интерфейс модуля
        if (!pModule->ReleaseInstance()) printf(" ReleaseInstance() --> Bad\n");
        else printf(" ReleaseInstance() --> OK\n");
        // обнулим указатель на интерфейс модуля
        pModule = NULL;
    }

    // выводим текст сообщения
    if (ErrorString) printf(ErrorString);

    // если нужно - аварийно завершаем программу
    if (TerminationFlag) exit(1);
    else return;
}

int TSUsb3000Reader::getQuietLevels(int *qlevels, int qtime){
    int avgs[3];
    for(int i = 0;i<qtime;i++)
    {
        if (pModule->READ_KADR(AdcBuffer)) {
            avgs[0]=AdcBuffer[0];
            avgs[1]=AdcBuffer[1];
            avgs[2]=AdcBuffer[2];
        }
        else
        {
            this->setLastError("Can`t read from device");
            return 16000;
        }
    }
    qlevels[0]=avgs[0]/qtime;
    qlevels[1]=avgs[1]/qtime;
    qlevels[2]=avgs[2]/qtime;
    return 0;
}

DWORD TSUsb3000Reader::read()
{
    WORD i;    // номер запроса на сбор данных
    WORD RequestNumber;    // идентификатор массива их двух событий
    HANDLE ReadEvent[2];    // массив OVERLAPPED структур из двух элементов
    OVERLAPPED ReadOv[2];
    DWORD BytesTransferred[2];
    DWORD TimeOut;

    // остановим ввод данных и одновременно прочистим соответствующий канал bulk USB
    //initDevice()
    //pModule->STOP_READ();
    //if(!pModule->STOP_READ()) {
    //  ThreadErrorNumber = 0x6;
    //IsThreadComplete = false;
    //  return 0;
    // }
    /*
    // создадим два события
    ReadEvent[0] = CreateEvent(NULL, FALSE , FALSE, NULL);
    memset(&ReadOv[0], 0, sizeof(OVERLAPPED)); ReadOv[0].hEvent = ReadEvent[0];
    ReadEvent[1] = CreateEvent(NULL, FALSE , FALSE, NULL);
    memset(&ReadOv[1], 0, sizeof(OVERLAPPED)); ReadOv[1].hEvent = ReadEvent[1];

    // таймаут ввода данных
    //	TimeOut = (DWORD)(DataStep/ReadRate + 1000);

    // делаем предварительный запрос на ввод данных
    RequestNumber = 0x0;
    if(!pModule->ReadData(ReadBuffer, &DataStep, &BytesTransferred[RequestNumber], &ReadOv[RequestNumber]))
        if(GetLastError() != ERROR_IO_PENDING) { CloseHandle(ReadEvent[0]); CloseHandle(ReadEvent[1]); ThreadErrorNumber = 0x2; IsThreadComplete = true; return 0; }

    // теперь запускаем ввод данных
    if(pModule->START_READ())
    {
        // цикл сбора данных
        for(i = 0x1; i < NBlockRead; i++)
        {
            RequestNumber ^= 0x1;
            // сделаем запрос на очередную порции данных
            if(!pModule->ReadData(ReadBuffer + i*DataStep, &DataStep, &BytesTransferred[RequestNumber], &ReadOv[RequestNumber]))
                if(GetLastError() != ERROR_IO_PENDING){
                    ThreadErrorNumber = 0x2; break;
                }

            // ждём окончания операции сбора очередной порции данных
            //if(!WaitingForRequestCompleted(&ReadOv[RequestNumber^0x1])) break;
            if(WaitForSingleObject(ReadEvent[!RequestNumber], TimeOut) == WAIT_TIMEOUT){
                ThreadErrorNumber = 0x3; break;
            }

            if(ThreadErrorNumber) break;
            else if(kbhit()) { ThreadErrorNumber = 0x1; break; }
            else Sleep(20);
            Counter++;
        }

        // ждём окончания операции сбора последней порции данных
        if(!ThreadErrorNumber)
        {
            RequestNumber ^= 0x1;
            //            WaitingForRequestCompleted(&ReadOv[RequestNumber^0x1]);
            if(WaitForSingleObject(ReadEvent[!RequestNumber], TimeOut) == WAIT_TIMEOUT)
                ThreadErrorNumber = 0x3;
            Counter++;
        }
    }
    else { ThreadErrorNumber = 0x5; }

    // остановим ввод данных
    if(!pModule->STOP_READ()) ThreadErrorNumber = 0x6;
    // если надо, то прервём незавершённый асинхронный запрос
    if(!CancelIo(pModule->GetModuleHandle())) ThreadErrorNumber = 0x7;
    // освободим все идентификаторы событий
    for(i = 0x0; i < 0x2; i++) CloseHandle(ReadEvent[i]);
    // небольшая задержка
    Sleep(100);
    // установим флажок окончания потока сбора данных
    IsThreadComplete = true;
    // теперь можно воходить из потока сбора данных
*/
    return 0;
}

void TSUsb3000Reader::setLastError(QString)
{
    ;
}

void TSUsb3000Reader::run()
{
    /*reader = new TSUsb3000Reader(this);
    reader->initDevice(buffer);*/
    /*QTimer tmr;
    switch (readingType)
    {
    case ReadAll:
    {
        qDebug()<<"read all "<<this->ReadingStarted;
        while(this->ReadingStarted)
        {
            SHORT* adc;
            if ((adc=readData())!=0){
                buffer->append(adc[0],adc[1],adc[2]);
                tmr.singleShot(10,this);
            }
            else{
                qDebug()<<"Get crashe";
                tmr.singleShot(10);
            }
            delete adc;
        }
        break;
    }
    case ReadForVolZer:
    {
        int avg=0;
        for(int i=0;i<300;i++)
        {
            SHORT* adc;
            if ((adc=readData())!=0){
                avg+=adc[0];
                if(i%10)
                    emit changeProgress(i/3);
                tmr.singleShot(10);
            }
            else{
                qDebug()<<"Get crashe";
                tmr.singleShot(10);
            }
            delete adc;
        }
        avg/=300;
        buffer->setVolumeColibration(avg,true);
        break;
    }
    case ReadForVolVal:
    {
        buffer->setEnd(0);
        for(int i=0;i<1200;i++)
        {
            SHORT* adc;
            if ((adc=readData())!=0){
                buffer->append(adc[0],0,0);
                if(i%24)
                    emit changeProgress(i/12);
                tmr.singleShot(10);
            }
            else{
                qDebug()<<"Get crashe";
                tmr.singleShot(10);
            }
            delete adc;
        }

        break;
    }
    }
    delete reader;
    reader = 0;*/
}

int TSUsb3000Reader::calibtateVolume()
{
    int avg=0;
    for(int i = 0;i<500;i++)
    {
        if (pModule->READ_KADR(AdcBuffer)) {
            avg+=AdcBuffer[0];
        }
        else
        {
            this->setLastError("Can`t read from device");
            return 16000;
        }
    }
    return avg/500;
}

TSUsb3000Reader::~TSUsb3000Reader()
{
    qDebug()<<closeReader();
}
bool TSUsb3000Reader::WaitingForRequestCompleted(OVERLAPPED *ReadOv)
{
    DWORD ReadBytesTransferred;

    while(true)
    {
        if(GetOverlappedResult(ModuleHandle, ReadOv, &ReadBytesTransferred, FALSE))
            break;
        else
            if(GetLastError() !=  ERROR_IO_INCOMPLETE){
                ThreadErrorNumber = 0x3;
                return false;
            }
            else
                if(kbhit()) {
                    ThreadErrorNumber = 0x1;
                    return false;
                }
                else
                    Sleep(20);
    }
    return true;
}
