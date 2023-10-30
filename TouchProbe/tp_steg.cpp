#include "tp_steg.h"
#include "ui_tp_steg.h"

TP_Steg::TP_Steg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TP_Steg)
{
    ui->setupUi(this);
}

TP_Steg::~TP_Steg()
{
    delete ui;
}
