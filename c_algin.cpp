#include "c_algin.h"
#include <QDir>
#include "ui_c_algin.h"

C_Algin::C_Algin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_Algin)
{
    ui->setupUi(this);
    ui->lineEdit_L2->installEventFilter(this);
    ui->textEdit->hide();

    stringList_Frames << "Oben" << "Hinten" << "Links" << "Vorne" << "Rechts";
    stringList_MessRichtung1 << "+" << "-";
    stringList_MessRichtung2 << "X" << "Y" << "Z";
    ui->comboBox_Frame->insertItems(0, stringList_Frames);
    ui->comboBox_Messrichtung1->insertItems(0, stringList_MessRichtung1);
    ui->comboBox_Messrichtung2->insertItems(0, stringList_MessRichtung2);
    ui->lineEdit_Positionierachse->setAlignment(Qt::AlignCenter);
    ui->comboBox_Frame->setCurrentIndex(1);
    ui->comboBox_Messrichtung1->setCurrentIndex(1);
    ui->comboBox_Messrichtung2->setCurrentIndex(2);
    ui->comboBox_Messrichtung2->setEditable(true);
    ui->comboBox_Messrichtung2->lineEdit()->setReadOnly(true);
    ui->comboBox_Messrichtung2->lineEdit()->setAlignment(Qt::AlignCenter);
    for (int i = 0 ; i < ui->comboBox_Messrichtung2->count() ; ++i) {
        ui->comboBox_Messrichtung2->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    ui->lineEdit_Positionierachse->setText("X");
    ui->comboBox_Messrichtung1->setEnabled(false);
    ui->comboBox_Messrichtung2->setEnabled(false);
    ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_ZMinus1.png"));
    SetComboBoxItemEnabled(ui->comboBox_Messrichtung2, 2, false);

    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));

    connect(ui->comboBox_Frame, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentFrameChanged(QString)));
    connect(ui->comboBox_Messrichtung1, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentTextChanged(QString)));
    connect(ui->comboBox_Messrichtung2, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentTextChanged(QString)));
    connect(ui->toolButton_Edit, SIGNAL(released()), this, SLOT(slot_create()));
    connect(ui->toolButton_Pix, SIGNAL(released()), this, SLOT(slot_pix()));
    connect(ui->toolButton_Export, SIGNAL(released()), this, SLOT(slot_export()));

    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/C_Algin/Anfahren.txt");
    mfile->read_Content();
    stringList_ContentAnfahren = mfile->get_Content();
    foreach(QString str, stringList_ContentAnfahren)
    {
        ui->textEdit_Anfahren->append(str);
    }
}

C_Algin::~C_Algin()
{
    delete ui;
}

bool C_Algin::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (object == ui->lineEdit_L2 && ui->comboBox_Frame->currentIndex() != 0)
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_ZMinus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XPlus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XMinus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YPlus2.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YMinus2.png"));
        }
    }

    if (event->type() == QEvent::FocusOut)
    {
        if (object == ui->lineEdit_L2 && ui->comboBox_Frame->currentIndex() != 0)
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_ZMinus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XPlus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XMinus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YPlus1.png"));
        }

        if (object == ui->lineEdit_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YMinus1.png"));
        }
    }

    return false;
}

/*
void C_Algin::set_Oben()
{

}
*/
void C_Algin::slot_currentFrameChanged(QString str)
{
    if(str == "Oben")
    {
        ui->comboBox_Messrichtung1->setEnabled(true);
        ui->comboBox_Messrichtung2->setEnabled(true);
        ui->label_Messrichtung->setEnabled(true);
        ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XPlus1.png"));
        ui->comboBox_Messrichtung1->setCurrentIndex(0);
        ui->comboBox_Messrichtung2->setCurrentIndex(0);
        ui->lineEdit_Positionierachse->setText("Y");
        SetComboBoxItemEnabled(ui->comboBox_Messrichtung2, 2, false);
    }
    else
    {
        ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_ZMinus1.png"));
        SetComboBoxItemEnabled(ui->comboBox_Messrichtung2, 2, true);
        ui->comboBox_Messrichtung1->setCurrentIndex(1);
        ui->comboBox_Messrichtung2->setCurrentIndex(2);
        ui->lineEdit_Positionierachse->setText("X");
        ui->comboBox_Messrichtung1->setEnabled(false);
        ui->comboBox_Messrichtung2->setEnabled(false);
        ui->label_Messrichtung->setEnabled(false);
    }
}

void C_Algin::slot_currentTextChanged(QString str)
{
    Q_UNUSED(str);

    if(ui->comboBox_Frame->currentText() != "Oben")
        return;

    if(ui->comboBox_Messrichtung1->currentText() == "+" &&
       ui->comboBox_Messrichtung2->currentText() == "X")
    {
       ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XPlus1.png"));
       ui->lineEdit_Positionierachse->setText("Y");
    }

    if(ui->comboBox_Messrichtung1->currentText() == "-" &&
        ui->comboBox_Messrichtung2->currentText() == "X")
    {
       ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XMinus1.png"));
       ui->lineEdit_Positionierachse->setText("Y");
    }

    if(ui->comboBox_Messrichtung1->currentText() == "+" &&
        ui->comboBox_Messrichtung2->currentText() == "Y")
    {
       ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YPlus1.png"));
       ui->lineEdit_Positionierachse->setText("X");
    }

    if(ui->comboBox_Messrichtung1->currentText() == "-" &&
        ui->comboBox_Messrichtung2->currentText() == "Y")
    {
       ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YMinus1.png"));
       ui->lineEdit_Positionierachse->setText("X");
    }
}

void C_Algin::SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled)
{
    auto * model = qobject_cast<QStandardItemModel*>(comboBox->model());
    assert(model);
    if(!model) return;

    auto * item = model->item(index);
    assert(item);
    if(!item) return;
    item->setEnabled(enabled);
}

void C_Algin::slot_create()
{
    //Lese stringList_Content aus dem kleinen textEdit_Anfahren
    stringList_ContentAnfahren = ui->textEdit_Anfahren->toPlainText().split("\n");

    //Loesche alles im Editor und schalte auf den Editor um
    ui->textEdit->clear();
    ui->label_Pix->hide();
    ui->textEdit->show();

    //Ausrichtzyklus wir fürs geschwenkte Antasten vorbereitet
    string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,$TSA$,103,2,$Lange$,,,,,1,0,1,)";

    //A und C Achsen definieren
    switch (ui->comboBox_Frame->currentIndex())
    {
       // Oben
        case 0:
           int_AAxis = 0;
           int_CAxis = 0;
           break;

        //Hinten
        case 1:
           int_AAxis = -90;
           int_CAxis = 0;
           break;

        //Links
        case 2:
           int_AAxis = -90;
           int_CAxis = 90;
           break;

        //Vorne
        case 3:
           int_AAxis = -90;
           int_CAxis = 180;
           break;

        //Rechts
        case 4:
           int_AAxis = -90;
           int_CAxis = -90;
           break;
    }

    //Zaehler auf 0 setzen
    counter = 0;

    //Kopf für das Schwenkprogramm laden und in den Editor
    //mittels der Funktion insert_Content einfürgen
    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/C_Algin/Header.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    insert_Content();

    //Body für das Schwenkprogramm laden und in den Editor
    //mittels der Funktion insert_Content einfürgen
    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/C_Algin/Body.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    insert_Content();


}

void C_Algin::slot_export()
{
    QString str;
    QDir dir;
    //qDebug() << project->get_ProgrammDir() << "/" << project->get_ProjectName() << "/" << project->get_ProjectFullName();
    //Allei Einstellungen übernehmen und das Programm im Editor neu erstellen
    slot_create();

    // Wenn es ProjecName oder ProjectFullName leer sind wird nur in das Programmverzeichnis geschrieben
    // Programme\_C_Ausrichten.SPF
    if(project->get_ProjectName().isEmpty() || project->get_ProjectFullName().isEmpty())
    {
           str = project->get_ProgrammDir() + "/_C_Ausrichten.SPF";
    }

    // Ansonsten wird in die Datei
    // Programme\E12345678.WPD\E12345678_E01_Sp2.WPD\_C_Ausrichten.SPF
    // geschrieben
    else
    {
        str = project->get_ProgrammDir() + "/"
            + project->get_ProjectName() + ".WPD/"
            + project->get_ProjectFullName() + ".WPD/_C_Ausrichten.SPF";

        // Wenn es das Verzeichnis
        // Programme\E12345678.WPD\E12345678_E01_Sp2.WPD
        // noch nicht gibt dann erstelle es
        dir.setPath(project->get_ProgrammDir() + "/"
                  + project->get_ProjectName() + ".WPD/"
                  + project->get_ProjectFullName() + ".WPD");

        if(!dir.exists())
        {
            dir.mkpath(project->get_ProgrammDir() + "/"
                       + project->get_ProjectName() + ".WPD/"
                       + project->get_ProjectFullName() + ".WPD");
        }
    }

    QFile file(str);

    if(file.open(QFile::WriteOnly))
    {
        //qDebug() << "File is open";
        //Oeffne stream und schreib jede Zeile aus textEdit in den Stream
        QTextStream stream(&file);
        foreach(QString string_Line, ui->textEdit->toPlainText().split("\n"))
        {
            qDebug() << string_Line;
            stream << string_Line << "\n";
        }
        file.close();
    }
}

void C_Algin::slot_pix()
{
    ui->label_Pix->show();
    ui->textEdit->hide();
}

void C_Algin::insert_Content()
{
    // Wenn Oben, X, +
    // setzte CYCL998
    if(ui->comboBox_Messrichtung1->currentIndex() == 0 &&
       ui->comboBox_Messrichtung2->currentIndex() == 0 &&
       ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,$TSA$,201,1,$Lange$,,,,,1,0,1,)";
    }

    // Wenn Oben, X, -
    // setzte CYCL998
    if(ui->comboBox_Messrichtung1->currentIndex() == 1 &&
        ui->comboBox_Messrichtung2->currentIndex() == 0 &&
        ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,$TSA$,201,2,$Lange$,,,,,1,0,1,)";
    }

    // Wenn Oben, Y, +
    // setzte CYCL998
    if(ui->comboBox_Messrichtung1->currentIndex() == 0 &&
        ui->comboBox_Messrichtung2->currentIndex() == 1 &&
        ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,$TSA$,102,1,$Lange$,,,,,1,0,1,)";
    }

    // Wenn Oben, Y, -
    // setzte CYCL998
    if(ui->comboBox_Messrichtung1->currentIndex() == 1 &&
        ui->comboBox_Messrichtung2->currentIndex() == 1 &&
        ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,$TSA$,102,2,$Lange$,,,,,1,0,1,)";
    }


    foreach (QString str, stringList_Content)
    {
       if(str.startsWith("#"))
         continue;

       counter++;
       //Header
       str = str.replace("$A$", QString("%1").arg(int_AAxis));
       str = str.replace("$C$", QString("%1").arg(int_CAxis));
       str = str.replace("$G5x$",  project->get_ProjectZeroPoint());

       //Body
       str = str.replace("$CYCLE998$", string_CYCLE998);
       str = str.replace("$Lange$", replace_Comma(ui->lineEdit_L2->text()));
       str = str.replace("$TSA$", replace_Comma(ui->lineEdit_TSA->text()));
       if(str.contains("$Anfahren$"))
       {
         foreach (QString str_Anfahren, stringList_ContentAnfahren)
         {
             str_Anfahren = QString("N%1 ").arg(counter) + str_Anfahren;
             ui->textEdit->append(str_Anfahren);
             counter++;
         }
         counter--;
       }
       else
       {
        str = QString("N%1 ").arg(counter) + str;
        ui->textEdit->append(str);
       }

    }
}

QString C_Algin::replace_Comma(QString str)
{
    QString returnString;
    returnString = str.replace(",",".");
    while(returnString.endsWith("0") && returnString.contains("."))
    {
       returnString = returnString.left(returnString.length()-1);
       qDebug() << returnString;
    }
    return returnString;
}
