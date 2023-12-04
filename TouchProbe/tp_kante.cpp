#include "tp_kante.h"
#include "ui_tp_kante.h"

TP_Kante::TP_Kante(QWidget *parent) :
    TP_Base(parent),
    ui(new Ui::TP_Kante)
{
    ui->setupUi(this);
    ui->lineEdit_Wert->installEventFilter(this);
    ui->lineEdit_DFA->installEventFilter(this);

    connect(ui->comboBox_Signs, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_AxesSignsChanged(QString)));
    connect(ui->comboBox_Axes,  SIGNAL(currentTextChanged(QString)), this, SLOT(slot_AxesSignsChanged(QString)));
}

TP_Kante::~TP_Kante()
{
    delete ui;
}

void TP_Kante::slot_AxesSignsChanged(QString str)
{
    Q_UNUSED(str);
    //emit sig_Err("test");

    ui->comboBox_Signs->setEnabled(true);
    if(ui->comboBox_Axes->currentText() == "Z")
    {
        ui->comboBox_Signs->setEnabled(false);
        ui->comboBox_Signs->setCurrentIndex(1);
        ui->label_Wert->setText("Z0");
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Z_Minus1.png"));
    }
    if(ui->comboBox_Signs->currentText() == "+" &&
        ui->comboBox_Axes->currentText() == "X")
    {
        ui->label_Wert->setText("X0");
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Plus1.png"));
    }

    if(ui->comboBox_Signs->currentText() == "-" &&
        ui->comboBox_Axes->currentText() == "X")
    {
        ui->label_Wert->setText("X0");
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Minus1.png"));
    }

    if(ui->comboBox_Signs->currentText() == "-" &&
        ui->comboBox_Axes->currentText() == "Y")
    {
        ui->label_Wert->setText("Y0");
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Minus1.png"));
    }

    if(ui->comboBox_Signs->currentText() == "+" &&
        ui->comboBox_Axes->currentText() == "Y")
    {
        ui->label_Wert->setText("Y0");
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Plus1.png"));
    }


}

bool TP_Kante::eventFilter(QObject *object, QEvent *ev)
{
    if(object == ui->lineEdit_Wert)
        eventFilter_Wert(ev);

    if(object == ui->lineEdit_DFA)
        eventFilter_DFA(ev);
    return false;
}

void TP_Kante::eventFilter_DFA(QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "+" &&
            ui->comboBox_Axes->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Plus3.png"));
        }
    }

    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "-" &&
            ui->comboBox_Axes->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Minus3.png"));
        }
    }

    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "+" &&
            ui->comboBox_Axes->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Plus3.png"));
        }
    }

    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "-" &&
            ui->comboBox_Axes->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Minus3.png"));
        }
    }

    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "-" &&
            ui->comboBox_Axes->currentText() == "Z")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Z_Minus3.png"));
        }
    }
}

void TP_Kante::eventFilter_Wert(QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "+" &&
            ui->comboBox_Axes->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Plus2.png"));
        }
    }


    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "-" &&
            ui->comboBox_Axes->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Minus2.png"));
        }
    }

    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "+" &&
            ui->comboBox_Axes->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Plus2.png"));
        }
    }

    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "-" &&
            ui->comboBox_Axes->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Minus2.png"));
        }
    }

    if (event->type() == QEvent::FocusIn)
    {
        if(ui->comboBox_Signs->currentText() == "-" &&
            ui->comboBox_Axes->currentText() == "Z")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Z_Minus2.png"));
        }
    }
}

void TP_Kante::slot_SignsChanged(QString str)
{

}

void TP_Kante::set_Anfahren()
{
    this->read_Anfahren();
    foreach(QString str, stringList_ContentAnfahren)
    {
        ui->textEdit_Anfahren->append(str);
    }
}

QStringList TP_Kante::postProcessing()
{
    QString string_CYCLE978;
    QString string_CYCL800;
    QString string_Axes;
    QString string_Frame;

    stringList_PostProcessing.clear();

    //Cyclus einrichten
    if(ui->comboBox_Signs->currentText() == "+" &&
        ui->comboBox_Axes->currentText() == "X")
    {
        string_CYCLE978 = "CYCLE978(100,9000,,1,$Wert$,$DFA$,$TSA$,1,1,1,\"\",,0,1.01,1.01,-1.01,0.34,1,0,,1,1)";
        string_Axes = "X+ ";
    }

    if(ui->comboBox_Signs->currentText() == "-" &&
        ui->comboBox_Axes->currentText() == "X")
    {
        string_CYCLE978 = "CYCLE978(100,9000,,1,$Wert$,$DFA$,$TSA$,1,2,1,\"\",,0,1.01,1.01,-1.01,0.34,1,0,,1,1)";
        string_Axes = "X- ";
    }

    if(ui->comboBox_Signs->currentText() == "+" &&
        ui->comboBox_Axes->currentText() == "Y")
    {
        string_CYCLE978 = "CYCLE978(100,9000,,1,$Wert$,$DFA$,$TSA$,2,1,1,\"\",,0,1.01,1.01,-1.01,0.34,1,0,,1,1)";
        string_Axes = "Y+ ";
    }

    if(ui->comboBox_Signs->currentText() == "-" &&
        ui->comboBox_Axes->currentText() == "Y")
    {
        string_CYCLE978 = "CYCLE978(100,9000,,1,$Wert$,$DFA$,$TSA$,2,2,1,\"\",,0,1.01,1.01,-1.01,0.34,1,0,,1,1)";
        string_Axes = "Y- ";
    }

    if(ui->comboBox_Signs->currentText() == "-" &&
        ui->comboBox_Axes->currentText() == "Z")
    {
        string_CYCLE978 = "CYCLE978(100,9000,,1,$Wert$,$DFA$,$TSA$,3,2,1,\"\",,0,1.01,1.01,-1.01,0.34,1,0,,1,1)";
        string_Axes = "Z- ";
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
    stringList_PostProcessing.append(";|                        Kante " + string_Axes + string_Frame + "                        |");
    stringList_PostProcessing.append(";+-----------------------------------------------------------------------+");
    stringList_PostProcessing.append("G75 Z1");
    stringList_PostProcessing.append(string_CYCL800);
    //Body_Kante
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Body_Kante.txt");
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
        str = str.replace("$CYCLE978$", string_CYCLE978);
        str = str.replace("$Wert$", replace_Comma(ui->lineEdit_Wert->text()));
        str = str.replace("$DFA$", replace_Comma(ui->lineEdit_DFA->text()));
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

void TP_Kante::setPixmap()
{
    if(ui->comboBox_Axes->currentText() == "Z")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Z_Minus1.png"));
    }

    if(ui->comboBox_Signs->currentText() == "+" &&
        ui->comboBox_Axes->currentText() == "X")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Plus1.png"));
    }

    if(ui->comboBox_Signs->currentText() == "-" &&
        ui->comboBox_Axes->currentText() == "X")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/X_Minus1.png"));
    }

    if(ui->comboBox_Signs->currentText() == "-" &&
        ui->comboBox_Axes->currentText() == "Y")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Minus1.png"));
    }

    if(ui->comboBox_Signs->currentText() == "+" &&
        ui->comboBox_Axes->currentText() == "Y")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Y_Plus1.png"));
    }
}
