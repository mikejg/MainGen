#include "tp_ebenheit.h"
#include "ui_tp_ebenheit.h"

TP_Ebenheit::TP_Ebenheit(QWidget *parent) :
    TP_Base(parent),
    ui(new Ui::TP_Ebenheit)
{
    ui->setupUi(this);
}

TP_Ebenheit::~TP_Ebenheit()
{
    delete ui;
}

QStringList TP_Ebenheit::postProcessing()
{
    QString string_Frame;

    stringList_PostProcessing.clear();

    switch(ui->comboBox_Frame->currentIndex())
    {
    case 0:
        string_Frame = "  Frame Oben  ";
        break;

    case 1:
        string_Frame = " Frame Hinten ";
        break;

    case 2:
        string_Frame = " Frame Links  ";
        break;

    case 3:
        string_Frame = "  Frame Vorne ";
        break;

    case 4:
        string_Frame = " Frame Rechts ";
        break;
    }

    //Tisch schwenken
    stringList_PostProcessing.append("                                                                          ");
    stringList_PostProcessing.append(";+-----------------------------------------------------------------------+");
    stringList_PostProcessing.append(";|                        Ebenheit " + string_Frame + "                        |");
    stringList_PostProcessing.append(";+-----------------------------------------------------------------------+");
    //stringList_PostProcessing.append("G75 Z1");
    //stringList_PostProcessing.append(string_CYCL800);

    //Body_Ebenheit
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Ebenheit.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    foreach(QString str, stringList_Content)
        stringList_PostProcessing.append(str);

    //StringList_PostProcessing Parsen
    /*QStringList stringList_Temp = stringList_PostProcessing;
    stringList_PostProcessing.clear();
    foreach (QString str, stringList_Temp)
    {
        stringList_PostProcessing.append(str);
    }*/


    return stringList_PostProcessing;
}

QStringList TP_Ebenheit::postProcessing_Ebenheit()
{
    qDebug() << Q_FUNC_INFO;

    stringList_Ebenheit.clear();
    QString string_CYCL800;
    QString string_Punkt;

    switch(ui->comboBox_Frame->currentIndex())
    {
    case 0:
        string_CYCL800= "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,0,0,0,0,0,0,-1,100,1)";
        break;

    case 1:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,0,-90,0,0,0,0,-1,100,1)";
        break;

    case 2:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,90,-90,0,0,0,0,-1,100,1)";
        break;

    case 3:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,180,-90,0,0,0,0,-1,100,1)";
        break;

    case 4:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,-90,-90,0,0,0,0,-1,100,1)";
        break;
    }


    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/_Ebenheit.SPF");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    foreach(QString str, stringList_Content)
    {
        if(str.startsWith("#"))
            continue;

        str = str.replace("$CYCL800$", string_CYCL800);
        str = str.replace("$Z_Ebene$", replace_Comma(ui->lineEdit_ZEbene->text()));
        str = str.replace("$VBereich$", replace_Comma(ui->lineEdit_Vertrauensbereich->text()));

        if(!ui->lineEdit_Punkt1X->text().isEmpty() && !ui->lineEdit_Punkt1Y->text().isEmpty())
        {
            string_Punkt = "Px = " + replace_Comma(ui->lineEdit_Punkt1X->text());
            str = str.replace("$Px$", string_Punkt);

            string_Punkt = "Py = " + replace_Comma(ui->lineEdit_Punkt1Y->text());
            str = str.replace("$Py$", string_Punkt);
        }

        if(!ui->lineEdit_Punkt2X->text().isEmpty() && !ui->lineEdit_Punkt2Y->text().isEmpty())
        {
            string_Punkt = "Px1 = " + replace_Comma(ui->lineEdit_Punkt2X->text());
            str = str.replace("$Px1$", string_Punkt);

            string_Punkt = "Py1 = " + replace_Comma(ui->lineEdit_Punkt2Y->text());
            str = str.replace("$Py1$", string_Punkt);
        }

        if(!ui->lineEdit_Punkt3X->text().isEmpty() && !ui->lineEdit_Punkt3Y->text().isEmpty())
        {
            string_Punkt = "Px2 = " + replace_Comma(ui->lineEdit_Punkt3X->text());
            str = str.replace("$Px2$", string_Punkt);

            string_Punkt = "Py2 = " + replace_Comma(ui->lineEdit_Punkt3Y->text());
            str = str.replace("$Py2$", string_Punkt);
        }
        else
        {
            str = str.replace("$Px2$", ";Px2 = 0 ; X für MessPunkt 2");
            str = str.replace("$Py2$", ";Py2 = 0 ; Y für MessPunkt 2");
        }

        if(!ui->lineEdit_Punkt4X->text().isEmpty() && !ui->lineEdit_Punkt4Y->text().isEmpty())
        {
            string_Punkt = "Px3 = " + replace_Comma(ui->lineEdit_Punkt4X->text());
            str = str.replace("$Px3$", string_Punkt);

            string_Punkt = "Py3 = " + replace_Comma(ui->lineEdit_Punkt4Y->text());
            str = str.replace("$Py3$", string_Punkt);
        }
        else
        {
            str = str.replace("$Px3$", ";Px3 = 0 ; X für MessPunkt 2");
            str = str.replace("$Py3$", ";Py3 = 0 ; Y für MessPunkt 2");
        }

        //qDebug() << str;
        stringList_Ebenheit.append(str);
    }

    return stringList_Ebenheit;
}

void TP_Ebenheit::setPixmap()
{
    emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ebenheit_01.png"));
}

