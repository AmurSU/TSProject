#include "tscontroller.h"
#include "ui_tsview.h"
#include <QtSql/QSqlRecord>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include "tools/tsvalidationtools.h"

TSController::TSController(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::TSView)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("CP-1251"));
    ui->setupUi(this);
    ui->mainBox->setCurrentIndex(0);
    currentAction = NoAction;
    nameRegExp = QRegExp(QString::fromLocal8Bit("[А-Я][а-я]{0,}"));
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(editPatientProfile()));
    connect(ui->saveProfileButton,SIGNAL(clicked()),this,SLOT(savePatientProfile()));
    connect(ui->ignoreCalibrateButton,SIGNAL(clicked()),this,SLOT(rejectColibration()));
}

TSController::~TSController()
{
    delete ui;
}

void TSController::incCurrentIndex()
{
    ui->mainBox->setCurrentIndex(ui->mainBox->currentIndex()+1);
}

void TSController::decCurrentIndex()
{
    ui->mainBox->setCurrentIndex(ui->mainBox->currentIndex()-1);
}

void TSController::editPatientProfile()
{
    switch(ui->mainBox->currentIndex())
    {
        case 0:
        {
            currentAction = CreatePatientProfileAction;
            ui->fName->clear();
            ui->sName->clear();
            ui->fdName->clear();
            ui->mvl->clear();
            ui->date->clear();
            ui->mGenderRadio->setChecked(true);
            break;
        }
        default: break;
    }
    ui->mainBox->setCurrentIndex(1);
}

void TSController::savePatientProfile()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(tr("Ошибка"));
    msgBox->setText(tr("Неправельный ввод данных."));
    switch(currentAction)
    {
        case CreatePatientProfileAction:
        {
            //some insert code
            QVariant value;
            QSqlRecord record;
            QString gender;
            if((value = TSValidationTools::isNameString(ui->sName->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле фамилия не должно содержать ничего кроме букв русского алфавита"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if((value = TSValidationTools::isNameString(ui->fName->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле имя не должно содержать ничего кроме букв русского алфавита"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if((value = TSValidationTools::isNameString(ui->fdName->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле отчество не должна содержать ничего кроме букв русского алфавита"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if((value = TSValidationTools::isInt(ui->mvl->text()))==false)
            {
                msgBox->setInformativeText(tr("Поле МВЛ должно содержать целое число"));
                msgBox->exec();
                return;
            }
            else
            {

            }
            if(ui->mGenderRadio->isChecked())
            {

            }
            if(ui->fGenderRadio->isChecked())
            {

            }
            ui->mainBox->setCurrentIndex(4);
            break;
        }

        case EditPatientProfileAction:
        {
            //some update code
            break;
        }
        default: break;
    }
    delete msgBox;
}

void TSController::rejectPatientProfile()
{

}

void TSController::calibrateVolume()
{

}

void TSController::calibrateTemperature()
{

}

void TSController::rejectColibration()
{
    ui->mainBox->setCurrentIndex(5);
    ui->managmentBox->setEnabled(true);
}
