#include "tsusbdatareader.h"
#include <QDebug>

TSUsbDataReader::TSUsbDataReader(QObject *parent) :
    QObject(parent)
{
    ReadRate = 1.0;
}

TSUsbDataReader::~TSUsbDataReader(){
}

bool TSUsbDataReader::initDevice(){
    //buffer=_bf;
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
        return false;
    } else
        printf(" Rtusbapi.dll Version --> OK\n");

    // получим указатель на интерфейс модуля USB3000
    pModule = static_cast<IRTUSB3000 *> (RtCreateInstance("usb3000"));
    if (!pModule){
        TerminateApplication(" Module Interface --> Bad\n");
        return false;
    }
    else
        printf(" Module Interface --> OK\n");

    // попробуем обнаружить модуль USB3000 в первых 127 виртуальных слотах
    for (i = 0x0; i < MaxVirtualSoltsQuantity; i++) if (pModule->OpenDevice(i)) break;
    // что-нибудь обнаружили?
    if (i == MaxVirtualSoltsQuantity){
        TerminateApplication(" Can't find module USB3000 in first 127 virtual slots!\n");
        return false;
    }
    else
        printf(" OpenDevice(%u) --> OK\n", i);

    // прочитаем название обнаруженного модуля
    if (!pModule->GetModuleName(ModuleName)) {
        TerminateApplication(" GetModuleName() --> Bad\n");
        return false;
    }
    else
        printf(" GetModuleName() --> OK\n");

    // проверим, что это 'USB3000'
    if (strcmp(ModuleName, "USB3000")){
        TerminateApplication(" The module is not 'USB3000'\n");
        return false;
    }
    else
        printf(" The module is 'USB3000'\n");

    // узнаем текущую скорость работы шины USB20
    if (!pModule->GetUsbSpeed(&UsbSpeed)) {
        printf(" GetUsbSpeed() --> Bad\n");
        return false;
    } else
        printf(" GetUsbSpeed() --> OK\n");
    // теперь отобразим версию драйвера AVR
    printf(" USB Speed is %s\n", UsbSpeed ? "HIGH (480 Mbit/s)" : "FULL (12 Mbit/s)");

    // прочитаем серийный номер модуля
    if (!pModule->GetModuleSerialNumber(ModuleSerialNumber)){
        TerminateApplication(" GetModuleSerialNumber() --> Bad\n");
        return false;
    }
    else
        printf(" GetModuleSerialNumber() --> OK\n");
    // теперь отобразим серийный номер модуля
    printf(" Module Serial Number is %s\n", ModuleSerialNumber);

    // прочитаем версию драйвера AVR
    if (!pModule->GetAvrVersion(AvrVersion)){
        TerminateApplication(" GetAvrVersion() --> Bad\n");
        return false;
    }
    else
        printf(" GetAvrVersion() --> OK\n");
    // теперь отобразим версию драйвера AVR
    printf(" Avr Driver Version is %s\n", AvrVersion);

    // код драйвера DSP возьмём из соответствующего ресурса штатной DLL библиотеки
    if (!pModule->LOAD_DSP()){
        TerminateApplication(" LOAD_DSP() --> Bad\n");
        return false;
    }
    else
        printf(" LOAD_DSP() --> OK\n");

    // проверим загрузку модуля
    if (!pModule->MODULE_TEST()){
        TerminateApplication(" MODULE_TEST() --> Bad\n");
        return false;
    }
    else
        printf(" MODULE_TEST() --> OK\n");

    // получим версию загруженного драйвера DSP
    if (!pModule->GET_DSP_INFO(&di)){
        TerminateApplication(" GET_DSP_INFO() --> Bad\n");
        return false;
    }
    else
        printf(" GET_DSP_INFO() --> OK\n");
    // теперь отобразим версию загруженного драйвера DSP
    printf(" DSP Driver version is %1u.%1u\n", di.DspMajor, di.DspMinor);

    // обязательно проинициализируем поле size структуры RTUSB3000::FLASH
    fi.size = sizeof (RTUSB3000::FLASH);
    // получим информацию из ППЗУ модуля
    if (!pModule->GET_FLASH(&fi)){
        TerminateApplication(" GET_FLASH() --> Bad\n");
        return false;
    }else
        printf(" GET_FLASH() --> OK\n");

    // обязательно проинициализируем поле size структуры RTUSB3000::INPUT_PARS
    ip.size = sizeof (RTUSB3000::INPUT_PARS);
    // получим текущие параметры работы АЦП
    if (!pModule->GET_INPUT_PARS(&ip)){
        TerminateApplication(" GET_INPUT_PARS() --> Bad\n");
        return false;
    }else
        printf(" GET_INPUT_PARS() --> OK\n");

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
    if (!pModule->SET_INPUT_PARS(&ip)){
        TerminateApplication(" SET_INPUT_PARS() --> Bad\n");
        return false;
    }else
        printf(" SET_INPUT_PARS() --> OK\n");

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
    return true;
}

void TSUsbDataReader::setReadingType(TSUsbReadingType type){
    readingType = type;
}

void TSUsbDataReader::read()
{
    SHORT* adc;
    ReadingStarted=true;
    switch (readingType){
    case ReadAll:{
        while(this->ReadingStarted)
        {
            if ( pModule ){
                if ((adc=readData())!=NULL){
                    buffer->append(adc[0],adc[1],adc[2]);
                    //printf("%d %d %d\n",adc[0],adc[1],adc[2]);
                    startTimer(10);
                }else{
                    qDebug()<<"Get crashe";
                    ReadingStarted=false;
                    startTimer(10);
                }
            }
        }
        break;
    }
    case ReadForVolZer:{
        int avg=0;
        qDebug()<<"Rading zero leav start";
        for(int i=0;i<300;i++){
            if ( pModule ){
                if ((adc=readData())!=NULL){
                    //qDebug()<<adc[0];
                    avg+=adc[0];
                    if(i%10)
                        emit changeProgress(i/3);
                    startTimer(10);
                }else{
                    qDebug()<<"Get crashe";
                    ReadingStarted=false;
                    startTimer(10);
                }
            }
        }
        qDebug()<<"Rading zero leav end";
        avg/=300;
        qDebug()<<"buffer->setVolumeColibration(avg,true);";
        buffer->setVolumeColibration(avg,true);
        qDebug()<<"break";
        break;
    }
    case ReadForVolVal:
    {
        buffer->setEnd(0);
        for(int i=0;i<1200;i++){
            if (ReadingStarted==false){
                i=1300;
            }
            if ( pModule ){
                if ((adc=readData())!=NULL){
                    buffer->append(adc[0],0,0);
                    if(i%24)
                        emit changeProgress(i/12);
                    startTimer(10);
                }else{
                    ReadingStarted=false;
                    qDebug()<<"Get crashe";
                    startTimer(10);
                }
            }
        }
        break;
    }
    }
    this->closeReader();
}

SHORT *TSUsbDataReader::readData()
{
    try{
        if ( pModule ){
            if (pModule->READ_KADR(AdcBuffer)) {
                return AdcBuffer;
            }
            else
            {
                this->setLastError("Can`t read from device");
                return NULL;
            }
        }
        return NULL;
    }catch(...){
        qDebug()<<"reading wrong";
    }
}

void TSUsbDataReader::stopRead(){
    ReadingStarted=false;
}

void TSUsbDataReader::doWork(){
    if ( this->initDevice() )
        this->read();
    emit done();
}
bool TSUsbDataReader::closeReader()
{
    pModule->STOP_READ();
    //qDebug()<<"close Reader";
    if (pModule) {
        // освободим интерфейс модуля
        if (!pModule->ReleaseInstance())
            printf(" ReleaseInstance() --> Bad\n");
        else
            printf(" ReleaseInstance() --> OK\n");
        // обнулим указатель на интерфейс модуля
        //qDebug()<<"pModule = NULL";
        pModule = NULL;
        return true;
    }
}


void TSUsbDataReader::TerminateApplication(char *ErrorString, bool TerminationFlag){
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
}

void TSUsbDataReader::setLastError(QString){
}

void TSUsbDataReader::setBuffer(TSCurveBuffer *bffr){
    buffer=bffr;
}


