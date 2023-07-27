#include "dialogProgress.h"
#include "ui_dialogProgress.h"

DialogProgress::DialogProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgress)
{
     ui->setupUi(this);
}

DialogProgress::~DialogProgress()
{
    delete ui;
}

void DialogProgress::initDialog(QStringList stringList)
{
    ui->progressBar->setMaximum(stringList.size());
}

void DialogProgress::setMaximum(int i)
{
    ui->progressBar->setMaximum(i);
}

void DialogProgress::setValue(QString string, int val)
{
    ui->label->setText(string);
    ui->progressBar->setValue(val);
    this->show();
}



