#include "tsview.h"
#include "ui_tsview.h"

TSView::TSView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TSView)
{
    ui->setupUi(this);
    paintTimer = 0;
}

TSView::~TSView()
{
    delete ui;
}

void TSView::showGUI()
{
    showMaximized();
}

void TSView::setController(TSController *c)
{
    control = c;
}
