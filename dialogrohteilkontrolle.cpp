#include "dialogrohteilkontrolle.h"
#include "ui_dialogrohteilkontrolle.h"

DialogRohteilkontrolle::DialogRohteilkontrolle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRohteilkontrolle)
{
    ui->setupUi(this);

    connect(ui->toolButton_RK00, SIGNAL(released()), this, SLOT(slot_Selected()));
    connect(ui->toolButton_RK01, SIGNAL(released()), this, SLOT(slot_Selected()));
    connect(ui->toolButton_RK02, SIGNAL(released()), this, SLOT(slot_Selected()));
}

DialogRohteilkontrolle::~DialogRohteilkontrolle()
{
    delete ui;
}

void DialogRohteilkontrolle::slot_Selected()
{
    if(sender() == ui->toolButton_RK00)
        project->set_RohteilKontrolle("Rohteilkontrolle00");
    if(sender() == ui->toolButton_RK01)
        project->set_RohteilKontrolle("Rohteilkontrolle01");
    if(sender() == ui->toolButton_RK02)
        project->set_RohteilKontrolle("Rohteilkontrolle02");

    emit sig_New_Rohteilkontrolle(project->get_RohteilKontrolle());
    this->close();
}
