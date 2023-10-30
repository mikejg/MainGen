#include "tp_base.h"
//#include "ui_tp_base.h"

/*TP_Base::TP_Base(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TP_Base)*/

TP_Base::TP_Base(QWidget *parent) :
      QWidget(parent)
{
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));

    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/C_Algin/Anfahren.txt");
    mfile->read_Content();
    stringList_ContentAnfahren = mfile->get_Content();
}

TP_Base::~TP_Base()
{

}
