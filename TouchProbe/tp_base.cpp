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


}

TP_Base::~TP_Base()
{

}

void TP_Base::read_Anfahren()
{
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Anfahren.txt");
    mfile->read_Content();
    stringList_ContentAnfahren = mfile->get_Content();
}

QString TP_Base::replace_Comma(QString str)
{
    QString returnString;
    returnString = str.replace(",",".");
    while(returnString.endsWith("0") && returnString.contains("."))
    {
        returnString = returnString.left(returnString.length()-1);
        //qDebug() << returnString;
    }
    return returnString;
}
