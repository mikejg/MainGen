#include "tp_item.h"
#include "ui_tp_item.h"


TP_Item::TP_Item(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TP_Item)
{
    ui->setupUi(this);
    hideAll();
    slot_Fold_In();
    state = NoState;
    connect(ui->widget_TP_Kante, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(ui->widget_TP_Kante, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(ui->widget_TP_Ausrichten, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(ui->widget_TP_Ausrichten, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(ui->widget_TP_Steg, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(ui->widget_TP_Steg, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));

    connect(ui->toolButton_Fold, SIGNAL(pressed()), this, SLOT(slot_Fold_Toggle()));

    connect(ui->toolButton_Steg, SIGNAL(pressed()), this, SLOT(slot_Show_TP_Steg()));
    connect(ui->toolButton_Kante, SIGNAL(pressed()), this, SLOT(slot_Show_TP_Kante()));
    connect(ui->toolButton_Ebenheit, SIGNAL(pressed()), this, SLOT(slot_Show_TP_Ebenheit()));
    connect(ui->toolButton_Ausrichten, SIGNAL(pressed()), this, SLOT(slot_Show_TP_Ausrichten()));
    connect(ui->toolButton_Bohrung, SIGNAL(pressed()), this, SLOT(slot_Show_TP_Bohrung()));
    connect(ui->toolButton_Add, SIGNAL(pressed()), this, SIGNAL(sig_NewItem()));
    connect(ui->toolButton_Trash, SIGNAL(pressed()), this, SLOT(slot_Delet_Item()));

    connect(ui->widget_TP_Kante, SIGNAL(sig_NewPixmap(QPixmap)),
            this, SIGNAL(sig_NewPixmap(QPixmap)));
    connect(ui->widget_TP_Ausrichten, SIGNAL(sig_NewPixmap(QPixmap)),
            this, SIGNAL(sig_NewPixmap(QPixmap)));
    connect(ui->widget_TP_Steg, SIGNAL(sig_NewPixmap(QPixmap)),
            this, SIGNAL(sig_NewPixmap(QPixmap)));
    connect(ui->widget_TP_Bohrung, SIGNAL(sig_NewPixmap(QPixmap)),
            this, SIGNAL(sig_NewPixmap(QPixmap)));
    connect(ui->widget_TP_Ebenheit, SIGNAL(sig_NewPixmap(QPixmap)),
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

    switch(state)
    {
        case Kante:
            ui->widget_TP_Kante->setPixmap();
            break;
        case Ausrichten:
            ui->widget_TP_Ausrichten->setPixmap();
            break;
        case Steg:
            ui->widget_TP_Steg->setPixmap();
            break;
        case Ebenheit:
            ui->widget_TP_Ebenheit->setPixmap();
            break;
        case Bohrung:
            ui->widget_TP_Bohrung->setPixmap();
            break;
    }
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
    hideAll();
    state = Kante;
    ui->widget_TP_Kante->show();
    ui->widget_TP_Kante->setPixmap();
    ui->label_PreView_Icon->setPixmap(QPixmap(QString::fromUtf8(":/Icons/TouchProbe/Kante.png")));
    ui->label_PreView_Text->setText("Antasten Kante");
}

void TP_Item::slot_Show_TP_Ebenheit()
{
    hideAll();
    state = Ebenheit;
    ui->widget_TP_Ebenheit->show();
    ui->widget_TP_Ebenheit->setPixmap();
    ui->label_PreView_Icon->setPixmap(QPixmap(QString::fromUtf8(":/Icons/TouchProbe/Ebene.png")));
    ui->label_PreView_Text->setText("Ebenheit");
}

void TP_Item::slot_Show_TP_Ausrichten()
{
    hideAll();
    state = Ausrichten;
    ui->widget_TP_Ausrichten->setPixmap();
    ui->widget_TP_Ausrichten->show();
    ui->label_PreView_Icon->setPixmap(QPixmap(QString::fromUtf8(":/Icons/TouchProbe/Ausrichten.png")));
    ui->label_PreView_Text->setText("Ausrichten");

}

void TP_Item::slot_Show_TP_Steg()
{
    hideAll();
    state = Steg;
    ui->widget_TP_Steg->setPixmap();
    ui->widget_TP_Steg->show();
    ui->label_PreView_Icon->setPixmap(QPixmap(QString::fromUtf8(":/Icons/TouchProbe/Steg.png")));
    ui->label_PreView_Text->setText("Steg");
}

void TP_Item::slot_Show_TP_Bohrung()
{
    hideAll();
    state = Bohrung;
    ui->widget_TP_Bohrung->setPixmap();
    ui->widget_TP_Bohrung->show();
    ui->label_PreView_Icon->setPixmap(QPixmap(QString::fromUtf8(":/Icons/TouchProbe/Bohrung.png")));
    ui->label_PreView_Text->setText("Bohrung");
}

void TP_Item::hideAll()
{
    ui->widget_TP_Kante->hide();
    ui->widget_TP_Ausrichten->hide();
    ui->widget_TP_Steg->hide();
    ui->widget_TP_Ebenheit->hide();
    ui->widget_TP_Bohrung->hide();
}

void TP_Item::read_Anfahren()
{
    ui->widget_TP_Kante->set_Anfahren();
    ui->widget_TP_Ausrichten->set_Anfahren();
    ui->widget_TP_Steg->set_Anfahren();
    ui->widget_TP_Bohrung->set_Anfahren();
}

QStringList TP_Item::postProcessing()
{
    stringList_PostProcessing.clear();

    if(state == Kante)
    {
        stringList_PostProcessing = ui->widget_TP_Kante->postProcessing();
    }

    if(state == Ausrichten)
    {
        stringList_PostProcessing = ui->widget_TP_Ausrichten->postProcessing();
    }

    if(state == Steg)
    {
        stringList_PostProcessing = ui->widget_TP_Steg->postProcessing();
    }

    if(state == Ebenheit)
    {
        stringList_PostProcessing = ui->widget_TP_Ebenheit->postProcessing();
    }

    if(state == Bohrung)
    {
        stringList_PostProcessing = ui->widget_TP_Bohrung->postProcessing();
    }
    return stringList_PostProcessing;
}

QStringList TP_Item::postProcessing_Ebenheit()
{
    qDebug() << Q_FUNC_INFO;
    stringList_PostProcessing_Ebenheit = ui->widget_TP_Ebenheit->postProcessing_Ebenheit();
    return stringList_PostProcessing_Ebenheit;
}

void TP_Item::slot_Delet_Item()
{
    emit sig_DeleteItem(this);
}
