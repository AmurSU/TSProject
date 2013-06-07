#include "tsusbdatareader.h"
#include <QDebug>

TSUsbDataReader::TSUsbDataReader(QObject *parent) :QObject(parent){
}

TSUsbDataReader::~TSUsbDataReader(){
}

bool TSUsbDataReader::initDevice(){
       try{
    //buffer=_bf;
    char ModuleName[10]; // �������� ������
    BYTE UsbSpeed; // �������� ������ ���� USB
    char ModuleSerialNumber[9]; // �������� ����� ������
    char AvrVersion[5]; // ������ �������� AVR
    RTUSB3000::DSP_INFO di; // ���������, ���������� ���������� � ������ �������� DSP
    RTUSB3000::FLASH fi; // ��������� ���������� � ���� ������
    RTUSB3000::INPUT_PARS ip; // ��������� ���������� ������ ���
    double ReadRate = 1.0; // �������  ����� ������
    const static WORD MaxVirtualSoltsQuantity = 4;
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
            qDebug("Dll version isn`t correct");
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
    // �������� ������ ������������ ���������� Rtusbapi.dll
    if ((DllVersion = RtGetDllVersion()) != CURRENT_VERSION_RTUSBAPI) {
        qDebug() << "Rtusbapi.dll Version --> bad";
        char String[128];
        sprintf(String, " Rtusbapi.dll Version Error!!!\n   Current: %1u.%1u. Required: %1u.%1u",
                DllVersion >> 0x10, DllVersion & 0xFFFF,
                CURRENT_VERSION_RTUSBAPI >> 0x10, CURRENT_VERSION_RTUSBAPI & 0xFFFF);
        return false;
    } else{
        qDebug(" Rtusbapi.dll Version --> OK\n");
        qDebug() << "Rtusbapi.dll Version --> ok";
    }

    // ������� ��������� �� ��������� ������ USB3000
    pModule = static_cast<IRTUSB3000 *> (RtCreateInstance("usb3000"));
    if (!pModule){
        qDebug(" Module Interface --> Bad\n");
        return false;
    }
    else
        qDebug(" Module Interface --> OK\n");

    // ��������� ���������� ������ USB3000 � ������ 127 ����������� ������
    for (i = 0x0; i < MaxVirtualSoltsQuantity; i++) if (pModule->OpenDevice(i)) break;
    // ���-������ ����������?

        if (i == MaxVirtualSoltsQuantity){
            qDebug(" Can't find module USB3000 in first 127 virtual slots!\n");
            return false;
        }
        else
            qDebug(" OpenDevice(%u) --> OK\n", i);
        qDebug(" FUck");


    // ��������� �������� ������������� ������
    if (!pModule->GetModuleName(ModuleName)) {
        qDebug(" GetModuleName() --> Bad\n");
        return false;
    }
    else
        qDebug(" GetModuleName() --> OK\n");

    // ��������, ��� ��� 'USB3000'
    if (strcmp(ModuleName, "USB3000")){
        qDebug(" The module is not 'USB3000'\n");
        return false;
    }
    else
        qDebug(" The module is 'USB3000'\n");

    // ������ ������� �������� ������ ���� USB20
    if (!pModule->GetUsbSpeed(&UsbSpeed)) {
        qDebug(" GetUsbSpeed() --> Bad\n");
        return false;
    } else
        qDebug(" GetUsbSpeed() --> OK\n");
    // ������ ��������� ������ �������� AVR
    qDebug(" USB Speed is %s\n", UsbSpeed ? "HIGH (480 Mbit/s)" : "FULL (12 Mbit/s)");

    // ��������� �������� ����� ������
    if (!pModule->GetModuleSerialNumber(ModuleSerialNumber)){
        qDebug(" GetModuleSerialNumber() --> Bad\n");
        return false;
    }
    else
        qDebug(" GetModuleSerialNumber() --> OK\n");
    // ������ ��������� �������� ����� ������
    qDebug(" Module Serial Number is %s\n", ModuleSerialNumber);

    // ��������� ������ �������� AVR
    if (!pModule->GetAvrVersion(AvrVersion)){
        qDebug(" GetAvrVersion() --> Bad\n");
        return false;
    }
    else
        qDebug(" GetAvrVersion() --> OK\n");
    // ������ ��������� ������ �������� AVR
    qDebug(" Avr Driver Version is %s\n", AvrVersion);

    // ��� �������� DSP ������ �� ���������������� ������� ������� DLL ����������
    if (!pModule->LOAD_DSP()){
        qDebug(" LOAD_DSP() --> Bad\n");
        return false;
    }
    else
        qDebug(" LOAD_DSP() --> OK\n");

    // �������� �������� ������
    if (!pModule->MODULE_TEST()){
        qDebug(" MODULE_TEST() --> Bad\n");
        return false;
    }
    else
        qDebug(" MODULE_TEST() --> OK\n");

    // ������� ������ ������������ �������� DSP
    if (!pModule->GET_DSP_INFO(&di)){
        qDebug(" GET_DSP_INFO() --> Bad\n");
        return false;
    }
    else
        qDebug(" GET_DSP_INFO() --> OK\n");
    // ������ ��������� ������ ������������ �������� DSP
    qDebug(" DSP Driver version is %1u.%1u\n", di.DspMajor, di.DspMinor);

    // ����������� ����������������� ���� size ��������� RTUSB3000::FLASH
    fi.size = sizeof (RTUSB3000::FLASH);
    // ������� ���������� �� ���� ������
    if (!pModule->GET_FLASH(&fi)){
        qDebug(" GET_FLASH() --> Bad\n");
        return false;
    }else
        qDebug(" GET_FLASH() --> OK\n");

    // ����������� ����������������� ���� size ��������� RTUSB3000::INPUT_PARS
    ip.size = sizeof (RTUSB3000::INPUT_PARS);
    // ������� ������� ��������� ������ ���
    if (!pModule->GET_INPUT_PARS(&ip)){
        qDebug(" GET_INPUT_PARS() --> Bad\n");
        return false;
    }else
        qDebug(" GET_INPUT_PARS() --> OK\n");

    // ��������� �������� ��������� ����� ������
    ip.CorrectionEnabled = true; // �������� ������������� ������
    ip.InputClockSource = RTUSB3000::INTERNAL_INPUT_CLOCK; // ����� ������������ ���������� �������� �������� ��� ����� ������
    //	ip.InputClockSource = RTUSB3000::EXTERNAL_INPUT_CLOCK;	// ����� ������������ ������� �������� �������� ��� ����� ������
    ip.SynchroType = RTUSB3000::NO_SYNCHRO; // �� ����� ������������ ������� ������������� ��� ����� ������
    //	ip.SynchroType = RTUSB3000::TTL_START_SYNCHRO;	// ����� ������������ �������� ������������� ������ ��� ����� ������
    ip.ChannelsQuantity = CHANNELS_QUANTITY; // ������ �������� ������
    for (i = 0x0; i < CHANNELS_QUANTITY; i++) ip.ControlTable[i] = (WORD) (i);
    ip.InputRate = ReadRate; // ������� ������ ��� � ���
    ip.InterKadrDelay = 0.0;
    // ����� ������������ ��������� ������������� ������������, ������� ��������� � ���� ������
    for (i = 0x0; i < 8; i++) {
        ip.AdcOffsetCoef[i] = fi.AdcOffsetCoef[i];
        ip.AdcScaleCoef[i] = fi.AdcScaleCoef[i];
    }
    // ��������� ��������� ��������� ������ ����� ������ � ������� DSP ������
    if (!pModule->SET_INPUT_PARS(&ip)){
        qDebug(" SET_INPUT_PARS() --> Bad\n");
        return false;
    }else
        qDebug(" SET_INPUT_PARS() --> OK\n");

    // ��������� ��������� ������ �� ������ ��������
    qDebug(" \n");
    qDebug(" Module USB3000 (S/N %s) is ready ... \n", fi.SerialNumber);
    qDebug(" Adc parameters:\n");
    qDebug("   InputClockSource is %s\n", ip.InputClockSource ? "EXTERNAL" : "INTERNAL");
    qDebug("   SynchroType is %s\n", ip.SynchroType ? "TTL_START_SYNCHRO" : "NO_SYNCHRO");
    qDebug("   ChannelsQuantity = %2d\n", ip.ChannelsQuantity);
    qDebug("   AdcRate = %8.3f kHz\n", ip.InputRate);
    //qDebug("   InterKadrDelay = %2.4f ms\n", ip.InterKadrDelay);
    qDebug("   ChannelRate = %8.3f kHz\n", ip.ChannelRate);
    qDebug("\n Press any key to terminate this program...\n");
    // ���� ������������� ���������� ������� ADC_KADR �
    // ����������� ���������� ������ �� ������ ������
     }catch(...){
         qDebug()<<"Exception";
     }
    //emit ������ � ��� ��� ����� ������
    return true;

}

void TSUsbDataReader::setReadingType(TSUsbReadingType type){
    readingType = type;
}

bool TSUsbDataReader::read(){
    qDebug()<<"read";
    SHORT* adc;
    switch (readingType){
    case ReadAll:{
        while( ReadingStarted )
            if ( pModule ){
                if ( (adc=readData()) != NULL )
                    buffer->append(adc[0],adc[1],adc[2]);
                else{
                    ReadingStarted = false;
                    qDebug()<<"ReadAll break";
                    return false;
                }
                startTimer(10);
                //qDebug()<<"ReadAll  pModule correct";
            }
        break;
    }
    case ReadForVolZer:{
        int avg=0;
        for(int i=0;i<300;i++){
            if ( pModule ){
                if ( (adc=readData()) != NULL ){
                    avg+=adc[0];
                    if( i%10 )
                        emit changeProgress(i/3);
                }else{
                    ReadingStarted=false;
                    qDebug()<<"ReadForVolZer break";
                    return false;
                }
                startTimer(10);
                qDebug()<<"ReadForVolZer  pModule correct";
            }
        }
        avg/=300;
        buffer->setVolumeColibration(avg,true);
        break;
    }
    case ReadForVolVal:{
        buffer->setEnd(0);
        for(int i=0;i<1200;i++){
            if ( ReadingStarted == false ){
                i=1300;
                break;
            }
            if ( pModule ){
                if ( (adc=readData()) != NULL ){
                    buffer->append(adc[0],0,0);
                    if( i%24 )
                        emit changeProgress(i/12);
                }else{
                    //ReadingStarted=false;
                    qDebug()<<"ReadForVolVal break";
                    return false;
                }
                qDebug()<<"ReadForVolVal  pModule correct";
                startTimer(10);
            }
        }
        ReadingStarted=false;
        break;
    }
    }
    return true;
}

SHORT *TSUsbDataReader::readData(){
    try{
        if ( pModule ){
            if (pModule->READ_KADR(AdcBuffer))
                return AdcBuffer;
            else{
                qDebug("Can`t read from device");
                return NULL;
            }
        }
        return NULL;
    }catch(...){
        qDebug()<<"reading wrong";
    }
}

void TSUsbDataReader::stopRead(){
    ReadingStarted = false;
}

void TSUsbDataReader::doWork(){

    if ( this->initDevice() == true ){
        qDebug()<<"debice is init";
        ReadingStarted=true;
        read();
        /*if ( read() == true )
            emit done(true);
        else
            emit done(false);*/

    }
    releaseReader();
    emit done();
}

void TSUsbDataReader::releaseReader(){
    qDebug()<<"releaseReader()";
    if ( pModule != NULL ){
        pModule->STOP_READ();
        pModule->ReleaseInstance();
        pModule = NULL;
    }
}

void TSUsbDataReader::setBuffer(TSCurveBuffer *bffr){
    buffer=bffr;
}

bool TSUsbDataReader::isReady(){
    if ( initDevice() == true ){
        qDebug()<<"init ne true";
        releaseReader();
        return true;
    }
    releaseReader();
    return false;
}


