#include "tp_bohrung.h"
#include "ui_tp_bohrung.h"

TP_Bohrung::TP_Bohrung(QWidget *parent) :
    TP_Base(parent),
    ui(new Ui::TP_Bohrung)
{
    ui->setupUi(this);

    ui->lineEdit_Durchmesser->installEventFilter(this);
}

TP_Bohrung::~TP_Bohrung()
{
    delete ui;
}

void TP_Bohrung::setPixmap()
{
    emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Bohrung01.png"));
}


bool TP_Bohrung::eventFilter(QObject *object, QEvent *ev)
{
    if(object == ui->lineEdit_Durchmesser)
        eventFilter_Durchmesser(ev);
    return false;
}

void TP_Bohrung::eventFilter_Durchmesser(QEvent* event)
{
    if (event->type() == QEvent::FocusIn)
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Bohrung02.png"));
    }

    if (event->type() == QEvent::FocusOut)
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Bohrung01.png"));
    }
}

void TP_Bohrung::set_Anfahren()
{
    this->read_Anfahren();
    foreach(QString str, stringList_ContentAnfahren)
    {
        ui->textEdit_Anfahren->append(str);
    }
}


QStringList TP_Bohrung::postProcessing()
{
    QString string_CYCLE977;
    QString string_CYCL800;
    QString string_Frame;

    stringList_PostProcessing.clear();

    string_CYCLE977 = "CYCLE977(101,9000,,1,$Durchmesser$,,,10,$TSA$,0,1,1,,,1,\"\",,0,0.2,0.1,-0.1,0.34,1,0,,1,1)";

    switch(ui->comboBox_Frame->currentIndex())
    {
    case 0:
        string_CYCL800= "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,0,0,0,0,0,0,-1,100,1)";
        string_Frame = "  Frame Oben  ";
        break;

    case 1:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,0,-90,0,0,0,0,-1,100,1)";
        string_Frame = " Frame Hinten ";
        break;

    case 2:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,90,-90,0,0,0,0,-1,100,1)";
        string_Frame = " Frame Links  ";
        break;

    case 3:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,180,-90,0,0,0,0,-1,100,1)";
        string_Frame = "  Frame Vorne ";
        break;

    case 4:
        string_CYCL800 = "CYCLE800(1,\"HERMLE\",100000,39,0,0,0,-90,-90,0,0,0,0,-1,100,1)";
        string_Frame = " Frame Rechts ";
        break;
    }

    //Tisch schwenken
    stringList_PostProcessing.append("                                                                          ");
    stringList_PostProcessing.append(";+-----------------------------------------------------------------------+");
    stringList_PostProcessing.append(";|                         Bohrung " + string_Frame + "                        |");
    stringList_PostProcessing.append(";+-----------------------------------------------------------------------+");
    stringList_PostProcessing.append("G75 Z1");
    stringList_PostProcessing.append(string_CYCL800);
    //Body_Bohrung
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Body_Bohrung.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    foreach(QString str, stringList_Content)
        stringList_PostProcessing.append(str);

    //StringList_PostProcessing Parsen
    QStringList stringList_Temp = stringList_PostProcessing;
    stringList_PostProcessing.clear();
    foreach (QString str, stringList_Temp)
    {
        if(str.startsWith("#"))
            continue;

        //str = str.replace("$G5x$",  project->get_ProjectZeroPoint());
        str = str.replace("$CYCLE977$", string_CYCLE977);
        str = str.replace("$Durchmesser$", replace_Comma(ui->lineEdit_Durchmesser->text()));
        //str = str.replace("$DFA$", replace_Comma(ui->lineEdit_DFA->text()));
        str = str.replace("$TSA$", replace_Comma(ui->lineEdit_TSA->text()));
        if(str.contains("$Anfahren$"))
        {
            foreach (QString str_Anfahren, ui->textEdit_Anfahren->toPlainText().split("\n"))
            {
                stringList_PostProcessing.append(str_Anfahren);
            }
        }
        else
        {
            stringList_PostProcessing.append(str);
        }
    }
    return stringList_PostProcessing;
}

