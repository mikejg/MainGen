#include "dialogrecover.h"
#include "ui_dialogrecover.h"
#include <QFileInfo>

DialogRecover::DialogRecover(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRecover)
{
    ui->setupUi(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(this);
     centralLayout->addWidget(ui->label);
     centralLayout->addWidget(ui->progressBar);


     timer = new QTimer(this);
     toolList = new ToolList(this);
}

DialogRecover::~DialogRecover()
{
    delete ui;
}

void DialogRecover::reset_Progress()
{
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->label->setText("");
}

void DialogRecover::setMaximum(int max)
{
    ui->progressBar->setMaximum(max);
}

void DialogRecover::setValue(QString string, int val)
{
    ui->label->setText(string);
    ui->progressBar->setValue(val);
    this->show();
}

void DialogRecover::slot_insertRPL()
{
    QString str = stringList.at(counter);
    QString string_RuestplanID;
    stringList_Split.clear();

    setValue(str, counter);
    QFileInfo fileInfo(str);
    stringList_Split = fileInfo.baseName().split("_");
    if(stringList_Split.size() < 4)
    {
        emit sig_Err("Falscher DateiName: " + fileInfo.baseName());
        emit sig_Err("Zeichnungsnummer_Zeichnungsstand_Spannung_Wiederholfertigung");
        emit sig_Err("Beispiel: E14207809_E02_Sp2_0");

        counter++;
        this->update();
        if(counter < stringList.size())
            timer->singleShot(500, this, SLOT(slot_insertRPL()));
        else
            this->hide();
        return;
    }

    string_Projekt = stringList_Split.at(0) + "_" +
                     stringList_Split.at(1) + "_" +
                     stringList_Split.at(2);

    int wf = dbManager->getWiederholFertigung(string_Projekt);
    string_WiederholFertigung = QString("%1").arg(wf);

    toolList->clear();
    parser->parse(str, toolList);

    /*Übergibt dem DBManager den Projektnamen und die toolList zum Eintragen*/
    dbManager->insertProject(string_Projekt + "_" + string_WiederholFertigung, toolList);

    /*
    //qDebug() << "Parser::parse( " << fileInfo.baseName() << ")";
    string_RuestplanID = dbManager->insertRuestplan(fileInfo.baseName());
    //qDebug() << "RuestplanID: " << string_RuestplanID;

    foreach(Tool* tool, toolList->getList())
    {
        dbManager->insertTool(tool, string_RuestplanID,fileInfo.baseName());
    }
    */

    counter++;
    this->update();
    if(counter < stringList.size())
        timer->singleShot(500, this, SLOT(slot_insertRPL()));
    else
        this->hide();
}

void DialogRecover::recover(QStringList strl)
{
   stringList = strl;
   ui->progressBar->setMaximum(stringList.size());
   counter = 0;
   this->show();
   timer->singleShot(0, this, SLOT(slot_insertRPL()));

}
