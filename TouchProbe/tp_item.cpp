#include "tp_item.h"
#include "ui_tp_item.h"


TP_Item::TP_Item(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TP_Item)
{
    ui->setupUi(this);
    ui->widget_TP_Kante->hide();
    slot_Fold_In();

    connect(ui->toolButton_Fold, SIGNAL(pressed()), this, SLOT(slot_Fold_Toggle()));
    connect(ui->toolButton_Kante, SIGNAL(pressed()), this, SLOT(slot_Show_TP_Kante()));
    connect(ui->widget_TP_Kante, SIGNAL(sig_NewPixmap(QPixmap)),
            this, SIGNAL(sig_NewPixmap(QPixmap)));
}

TP_Item::~TP_Item()
{
    delete ui;
}

void TP_Item::setText(QString str)
{
    ui->label_PreView_Text->setText(str);
}

QString TP_Item::getText()
{
    return ui->label_PreView_Text->text();
}

void TP_Item::slot_Fold_In()
{
    ui->widget_FullView->hide();
    bool_Fold = false;
}

void TP_Item::slot_Fold_Out()
{
    ui->widget_FullView->show();
    bool_Fold = true;
}

void TP_Item::slot_Fold_Toggle()
{
    if(bool_Fold)
        slot_Fold_In();
    else
        slot_Fold_Out();
}

void TP_Item::slot_Show_TP_Kante()
{
    ui->widget_TP_Kante->show();
    ui->label_PreView_Icon->setPixmap(QPixmap(QString::fromUtf8(":/Icons/TouchProbe/Kante.png")));
    ui->label_PreView_Text->setText("Antasten Kante");
    emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Plus1.png"));
}
