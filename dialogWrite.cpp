#include "dialogWrite.h"
#include "ui_dialogWrite.h"

DialogWrite::DialogWrite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWrite)
{
     ui->setupUi(this);
    /**
    QVBoxLayout *centralLayout = new QVBoxLayout(this);
     centralLayout->addWidget(ui->label);
     centralLayout->addWidget(ui->progressBar);   */
}

DialogWrite::~DialogWrite()
{
    delete ui;
}

void DialogWrite::initDialog(QStringList stringList)
{
    ui->progressBar->setMaximum(stringList.size());
}

void DialogWrite::setValue(QString string, int val)
{
    ui->label->setText(string);
    ui->progressBar->setValue(val);
    this->show();
}


void DialogWrite::finish_Antasten()
{
/*
    QStringList stringList_Lines;
    QFile file_fileToParse(string_DST + "/_Sp1_Rohteil_antasten.SPF");
    QString string_Line;

    if(!file_fileToParse.open(QFile::ReadOnly))
    {
        string_Log = Q_FUNC_INFO + QString(" oeffnen von Sp1_Rohteil_antasten.SPF fehlgeschlagen");
        emit sig_Err(string_Log);
        return;
    }


    QTextStream in(&file_fileToParse);
    stringList_Lines.clear();
    while (!in.atEnd())
    {
        string_Line = in.readLine();
        if(string_Line.startsWith("#"))
            continue;
        string_Line = string_Line.replace("$RX$", dialogStart->lineEdit_RohteilX->text());
        string_Line = string_Line.replace("$RY$", dialogStart->lineEdit_RohteilY->text());
        string_Line = string_Line.replace("$ZRT$", QString("%1").arg(dialogStart->doubleSpinBox_ZRohTeil->value()));
        stringList_Lines.append(string_Line);
    }
    file_fileToParse.close();

    file_fileToParse.remove();
    if(file_fileToParse.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream textStream(&file_fileToParse);
        foreach(QString str, stringList_Lines)
        {
                textStream << str << '\n';
        }
        file_fileToParse.close();
    }
*/
}

void DialogWrite::finish_Rohteil_Kontrolle()
{


}

void DialogWrite::finish_Reinigen()
{

}

