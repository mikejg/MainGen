#include "touchprobe.h"
#include "ui_touchprobe.h"

TouchProbe::TouchProbe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TouchProbe)
{
    ui->setupUi(this);
    ui->textEdit->hide();
    ui->scrollAreaWidgetContents->set_Layout(ui->verticalLayout_scrollArea);
    ui->scrollAreaWidgetContents->set_SpacerItem(ui->verticalSpacer_ScrollContent);
    ui->scrollAreaWidgetContents->insert_Item("Item 1");
    ui->scrollAreaWidgetContents->insert_Item("Item 2");
    ui->scrollAreaWidgetContents->insert_Item("Item 1");
    ui->scrollAreaWidgetContents->insert_Item("Item 2");


    connect(ui->scrollAreaWidgetContents, SIGNAL(sig_NewPixmap(QPixmap)),
            this, SLOT(slot_NewPixmap(QPixmap)));
}

TouchProbe::~TouchProbe()
{
    delete ui;
}

void TouchProbe::slot_NewPixmap(QPixmap p)
{
    ui->label_Pix->setPixmap(p);
}
