#include "dialogWrite.h"
#include "ui_dialogWrite.h"

DialogWrite::DialogWrite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWrite)
{
    ui->setupUi(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(this);
     centralLayout->addWidget(ui->label);
     centralLayout->addWidget(ui->progressBar);

     timer = new QTimer(this);
}

DialogWrite::~DialogWrite()
{
    delete ui;
}

void DialogWrite::setValue(QString string, int val)
{
    ui->label->setText(string);
    ui->progressBar->setValue(val);
    this->show();
}

void DialogWrite::initDialog(bool b)
{
    /*Zähler wird auf Null gesetzt
     * Die Länge des Fortschritsbalken ist die anzahl der Programme
     * Zeige den Dialog an
     * starte slot_nextProgramm*/
    counter = 0;
    ui->progressBar->setMaximum(stringList_Programme.size());
    this->show();
    timer->singleShot(0, this, SLOT(slot_nextProgramm()));
    bool_Numbering = b;
}

void DialogWrite::slot_nextProgramm()
{
    string_Programm = stringList_Programme.at(counter);
    setValue(string_Programm, counter);

    parser_Programm->finish(string_ProgrammDir+ "/" + string_Programm);

    QFile::copy(string_ProgrammDir + QDir::separator() + string_Programm,
                string_DST + QDir::separator() + string_Programm);
    file.setFileName(string_ProgrammDir + QDir::separator() + string_Programm);
    file.remove();

    counter++;
    this->update();
    if(counter < stringList_Programme.size())
        timer->singleShot(500, this, SLOT(slot_nextProgramm()));
    else
    {
        if(!bool_Numbering)
        {
            QFile::copy(string_ProgrammDir + QDir::separator() + string_Projekt +".MPF",
                    string_DST + QDir::separator() + string_Projekt +".MPF");
            file.setFileName(string_ProgrammDir + QDir::separator() + string_Projekt +".MPF");
            file.remove();
        }
        else
        {
            QFile::copy(string_ProgrammDir + QDir::separator() + string_Projekt +".MPF",
                    string_DST + QDir::separator() + "00_" + string_Projekt +".MPF");
            file.setFileName(string_ProgrammDir + QDir::separator() + string_Projekt +".MPF");
            file.remove();
        }

        if(string_DST.contains("Sp1"))
        {
            finish_Antasten();
            finish_Rohteil_Kontrolle();
            finish_Reinigen();
        }
        this->hide();
        emit sig_Finished();
    }
}

void DialogWrite::finish_Antasten()
{

    QStringList stringList_Lines;
    QFile file_fileToParse(string_DST + "/_Sp1_Rohteil_antasten.SPF");
    QString string_Line;

    if(!file_fileToParse.open(QFile::ReadOnly))
    {
        string_Log = Q_FUNC_INFO + QString(" oeffnen von Sp1_Rohteil_antasten.SPF fehlgeschlagen");
        sig_Err(string_Log);
        return;
    }


    QTextStream in(&file_fileToParse);
    stringList_Lines.clear();
    while (!in.atEnd())
    {
        string_Line = in.readLine();
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
}

void DialogWrite::finish_Rohteil_Kontrolle()
{
    QStringList stringList_Lines;
    QFile file_fileToParse(string_DST + "/_Sp1_Rohteilkontrolle.SPF");
    QString string_Line;

    if(!file_fileToParse.open(QFile::ReadOnly))
    {
        string_Log = Q_FUNC_INFO + QString(" oeffnen von Sp1_Rohteilkontrolle.SPF fehlgeschlagen");
        sig_Err(string_Log);
        return;
    }

    QTextStream in(&file_fileToParse);
    stringList_Lines.clear();
    while (!in.atEnd())
    {
        string_Line = in.readLine();
        string_Line = string_Line.replace("$RX$", dialogStart->lineEdit_RohteilX->text());
        string_Line = string_Line.replace("$RY$", dialogStart->lineEdit_RohteilY->text());
        string_Line = string_Line.replace("$RZ$", dialogStart->lineEdit_RohteilZ->text());
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
}

void DialogWrite::finish_Reinigen()
{
    QStringList stringList_Lines;
    QFile file_fileToParse(string_DST + "/_Sp1_Reinigen.SPF");
    QString string_Line;

    if(!file_fileToParse.open(QFile::ReadOnly))
    {
        string_Log = Q_FUNC_INFO + QString(" oeffnen von Sp1_Reinigen.SPF fehlgeschlagen");
        sig_Err(string_Log);
        return;
    }

    QTextStream in(&file_fileToParse);
    stringList_Lines.clear();
    while (!in.atEnd())
    {
        string_Line = in.readLine();
        string_Line = string_Line.replace("$RX$", dialogStart->lineEdit_RohteilX->text());
        string_Line = string_Line.replace("$RY$", dialogStart->lineEdit_RohteilY->text());
        string_Line = string_Line.replace("$RZ$", dialogStart->lineEdit_RohteilZ->text());
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
}
