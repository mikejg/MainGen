#include "dialogrepetition.h"
#include "ui_dialogrepetition.h"

DialogRepetition::DialogRepetition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRepetition)
{
    ui->setupUi(this);
    ui->radioButton_Paket->setChecked(true);

    radioButton_Paket = ui->radioButton_Paket;
    radioButton_Repetition = ui->radioButton_Import;
}

DialogRepetition::~DialogRepetition()
{
    delete ui;
}
