#include "tp_steg.h"
#include "ui_tp_steg.h"

TP_Steg::TP_Steg(QWidget *parent) :
    TP_Base(parent),
    ui(new Ui::TP_Steg)
{
    ui->setupUi(this);
    ui->lineEdit_W->installEventFilter(this);
    ui->lineEdit_DFA->installEventFilter(this);
    ui->lineEdit_DZ->installEventFilter(this);

    connect(ui->comboBox_Messachse, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_AxesChanged(QString)));
}

TP_Steg::~TP_Steg()
{
    delete ui;    
}

void TP_Steg::set_Anfahren()
{
    this->read_Anfahren();
    foreach(QString str, stringList_ContentAnfahren)
    {
        ui->textEdit_Anfahren->append(str);
    }
}

void TP_Steg::setPixmap()
{
    if(ui->comboBox_Messachse->currentText() == "X")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_X1.png"));
    }

    if(ui->comboBox_Messachse->currentText() == "Y")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_Y1.png"));
    }
}

void TP_Steg::slot_AxesChanged(QString str)
{
    Q_UNUSED(str);

    if(ui->comboBox_Messachse->currentText() == "X")
       emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_X1.png"));

    if(ui->comboBox_Messachse->currentText() == "Y")
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_Y1.png"));
}

bool TP_Steg::eventFilter(QObject *object, QEvent *ev)
{
    if(object == ui->lineEdit_W)
        eventFilter_Wert(ev);

    if(object == ui->lineEdit_DFA)
        eventFilter_DFA(ev);
    if(object == ui->lineEdit_DZ)
        eventFilter_DZ(ev);
    return false;
}

void TP_Steg::eventFilter_Wert(QEvent* event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Messachse->currentText() == "X")
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_X2.png"));

        if(ui->comboBox_Messachse->currentText() == "Y")
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_Y2.png"));
    }
}

void TP_Steg::eventFilter_DFA(QEvent* event)
{
    if (event->type() == QEvent::FocusIn)
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_DFA.png"));
    }
}

void TP_Steg::eventFilter_DZ(QEvent* event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Messachse->currentText() == "X")
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_X3.png"));

        if(ui->comboBox_Messachse->currentText() == "Y")
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Steg_Y3.png"));
    }
}

QStringList TP_Steg::postProcessing()
{
    QString string_CYCLE977;
    QString string_CYCL800;
    QString string_Axes;
    QString string_Frame;

    stringList_PostProcessing.clear();

    //Cyclus einrichten
    if(ui->comboBox_Messachse->currentText() == "X")
    {
        string_CYCLE977 = "CYCLE977(104,9000,,1,$Lange$,,,$DFA$,$TSA$,0,$DZ$,1,,1,1,\"\",,0,1.01,1.01,-1.01,0.34,1,0,,1,1)";
        string_Axes = "X";
    }

    if(ui->comboBox_Messachse->currentText() == "Y")
    {
        string_CYCLE977 = "CYCLE977(104,9000,,1,$Lange$,,,$DFA$,$TSA$,0,$DZ$,1,,2,1,\"\",,0,1.01,1.01,-1.01,0.34,1,0,,1,1)";
        string_Axes = "Y";
    }

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
    stringList_PostProcessing.append(";|                          Steg " + string_Axes + string_Frame + "                         |");
    stringList_PostProcessing.append(";+-----------------------------------------------------------------------+");
    stringList_PostProcessing.append("G75 Z1");
    stringList_PostProcessing.append(string_CYCL800);
    //Body_Kante
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Body_Steg.txt");
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
        str = str.replace("$Lange$", replace_Comma(ui->lineEdit_W->text()));
        str = str.replace("$DFA$", replace_Comma(ui->lineEdit_DFA->text()));
        str = str.replace("$TSA$", replace_Comma(ui->lineEdit_TSA->text()));
        str = str.replace("$DZ$", replace_Comma(ui->lineEdit_DZ->text()));
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
