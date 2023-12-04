#include "tp_ausrichten.h"
#include "ui_tp_ausrichten.h"

TP_Ausrichten::TP_Ausrichten(QWidget *parent) :
    TP_Base(parent),
    ui(new Ui::TP_Ausrichten)
{
    ui->setupUi(this);
    ui->lineEdit_L2->installEventFilter(this);
    ui->lineEdit_Positionierachse->setAlignment(Qt::AlignCenter);
    ui->comboBox_Frame->setCurrentIndex(1);
    ui->comboBox_Messrichtung1->setCurrentIndex(1);
    ui->comboBox_Messrichtung2->setCurrentIndex(2);
    ui->comboBox_Messrichtung2->setEditable(true);
    ui->comboBox_Messrichtung2->lineEdit()->setReadOnly(true);
    ui->comboBox_Messrichtung2->lineEdit()->setAlignment(Qt::AlignCenter);
    for (int i = 0 ; i < ui->comboBox_Messrichtung2->count() ; ++i)
    {
        ui->comboBox_Messrichtung2->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    ui->lineEdit_Positionierachse->setText("X");
    ui->comboBox_Messrichtung1->setEnabled(false);
    ui->comboBox_Messrichtung2->setEnabled(false);
    SetComboBoxItemEnabled(ui->comboBox_Messrichtung2, 2, false);

    connect(ui->comboBox_Frame, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentFrameChanged(QString)));
    connect(ui->comboBox_Messrichtung1, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentTextChanged(QString)));
    connect(ui->comboBox_Messrichtung2, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentTextChanged(QString)));
}

TP_Ausrichten::~TP_Ausrichten()
{
    delete ui;
}

void TP_Ausrichten::set_Anfahren()
{
    this->read_Anfahren();
    foreach(QString str, stringList_ContentAnfahren)
    {
        ui->textEdit_Anfahren->append(str);
    }
}

void TP_Ausrichten::SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled)
{
    auto * model = qobject_cast<QStandardItemModel*>(comboBox->model());
    assert(model);
    if(!model) return;

    auto * item = model->item(index);
    assert(item);
    if(!item) return;
    item->setEnabled(enabled);
}

void TP_Ausrichten::slot_currentFrameChanged(QString str)
{
    if(str == "Oben")
    {
        ui->comboBox_Messrichtung1->setEnabled(true);
        ui->comboBox_Messrichtung2->setEnabled(true);
        ui->label_Messrichtung->setEnabled(true);
         emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XPlus1.png"));
        ui->comboBox_Messrichtung1->setCurrentIndex(0);
        ui->comboBox_Messrichtung2->setCurrentIndex(0);
        ui->lineEdit_Positionierachse->setText("Y");
        SetComboBoxItemEnabled(ui->comboBox_Messrichtung2, 2, false);
    }
    else
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_ZMinus1.png"));
        SetComboBoxItemEnabled(ui->comboBox_Messrichtung2, 2, true);
        ui->comboBox_Messrichtung1->setCurrentIndex(1);
        ui->comboBox_Messrichtung2->setCurrentIndex(2);
        ui->lineEdit_Positionierachse->setText("X");
        ui->comboBox_Messrichtung1->setEnabled(false);
        ui->comboBox_Messrichtung2->setEnabled(false);
        ui->label_Messrichtung->setEnabled(false);
    }
}

void TP_Ausrichten::slot_currentTextChanged(QString str)
{
    Q_UNUSED(str);

    if(ui->comboBox_Frame->currentText() != "Oben")
        return;

    if(ui->comboBox_Messrichtung1->currentText() == "+" &&
        ui->comboBox_Messrichtung2->currentText() == "X")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XPlus1.png"));
        ui->lineEdit_Positionierachse->setText("Y");
    }

    if(ui->comboBox_Messrichtung1->currentText() == "-" &&
        ui->comboBox_Messrichtung2->currentText() == "X")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XMinus1.png"));
        ui->lineEdit_Positionierachse->setText("Y");
    }

    if(ui->comboBox_Messrichtung1->currentText() == "+" &&
        ui->comboBox_Messrichtung2->currentText() == "Y")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YPlus1.png"));
        ui->lineEdit_Positionierachse->setText("X");
    }

    if(ui->comboBox_Messrichtung1->currentText() == "-" &&
        ui->comboBox_Messrichtung2->currentText() == "Y")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YMinus1.png"));
        ui->lineEdit_Positionierachse->setText("X");
    }
}

bool TP_Ausrichten::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (object == ui->lineEdit_L2 && ui->comboBox_Frame->currentIndex() != 0)
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_ZMinus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XPlus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XMinus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YPlus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YMinus2.png"));
        }
    }

    if (event->type() == QEvent::FocusOut)
    {
        if (object == ui->lineEdit_L2 && ui->comboBox_Frame->currentIndex() != 0)
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_ZMinus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XPlus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XMinus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YPlus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YMinus1.png"));
        }
    }

    return false;
}

QStringList TP_Ausrichten::postProcessing()
{
    QString string_CYCLE998;
    QString string_CYCL800;
    //QString string_Axes;
    QString string_Frame;

    stringList_PostProcessing.clear();

    //Ausrichtzyklus wir fürs geschwenkte Antasten vorbereitet
    string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,$TSA$,103,2,$Lange$,,,,,1,0,1,)";

    //A und C Achsen definieren
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
    stringList_PostProcessing.append(";|                       Ausrichten " + string_Frame + "                       |");
    stringList_PostProcessing.append(";+-----------------------------------------------------------------------+");
    stringList_PostProcessing.append("G75 Z1");
    stringList_PostProcessing.append(string_CYCL800);

    //Body_Ausrichten
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Body_Ausrichten.txt");
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
        str = str.replace("$CYCLE998$", string_CYCLE998);
        str = str.replace("$Lange$", replace_Comma(ui->lineEdit_L2->text()));
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

void TP_Ausrichten::setPixmap()
{
    if(ui->comboBox_Frame->currentText() != "Oben")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_ZMinus1.png"));
        return;
    }

    if (ui->comboBox_Messrichtung1->currentText() == "+" &&
        ui->comboBox_Messrichtung2->currentText() == "X")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XPlus1.png"));
    }

    if (ui->comboBox_Messrichtung1->currentText() == "-" &&
        ui->comboBox_Messrichtung2->currentText() == "X")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_XMinus1.png"));
    }

    if (ui->comboBox_Messrichtung1->currentText() == "+" &&
        ui->comboBox_Messrichtung2->currentText() == "Y")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YPlus1.png"));
    }

    if (ui->comboBox_Messrichtung1->currentText() == "-" &&
        ui->comboBox_Messrichtung2->currentText() == "Y")
    {
        emit sig_NewPixmap(QPixmap(":/Icons/TouchProbe/Ausrichten_YMinus1.png"));
    }
}
