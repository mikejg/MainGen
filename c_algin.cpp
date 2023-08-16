#include "c_algin.h"
#include <QDir>
#include "ui_c_algin.h"

C_Algin::C_Algin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_Algin)
{
    ui->setupUi(this);
    ui->spinBox_L2->installEventFilter(this);
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
}

C_Algin::~C_Algin()
{
    delete ui;
}

bool C_Algin::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (object == ui->spinBox_L2 && ui->comboBox_Frame->currentIndex() != 0)
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_ZMinus2.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XPlus2.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XMinus2.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YPlus2.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YMinus2.png"));
        }
    }

    if (event->type() == QEvent::FocusOut)
    {
        if (object == ui->spinBox_L2 && ui->comboBox_Frame->currentIndex() != 0)
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_ZMinus1.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XPlus1.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "X")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_XMinus1.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "+" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YPlus1.png"));
        }

        if (object == ui->spinBox_L2 &&
            ui->comboBox_Frame->currentText() == "Oben" &&
            ui->comboBox_Messrichtung1->currentText() == "-" &&
            ui->comboBox_Messrichtung2->currentText() == "Y")
        {
            ui->label_Pix->setPixmap(QPixmap(":/Icons/CAlgin/C_Algin_YMinus1.png"));
        }
    }

    return false;
}

void C_Algin::set_Oben()
{

}

void C_Algin::slot_currentFrameChanged(QString str)
{
    if(str == "Oben")
    {
        ui->comboBox_Messrichtung1->setEnabled(true);
        ui->comboBox_Messrichtung2->setEnabled(true);
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
    QString str;
    double double_Temp;

    ui->textEdit->clear();
    ui->label_Pix->hide();
    ui->textEdit->show();

    //Kommas in Punkt umwandeln
    ui->lineEdit_X->setText(replace_Comma(ui->lineEdit_X->text()));
    ui->lineEdit_Y->setText(replace_Comma(ui->lineEdit_Y->text()));
    ui->lineEdit_Z->setText(replace_Comma(ui->lineEdit_Z->text()));

    string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,0.2,103,2,$Lange$,,,,,1,0,1,)";
    //A und C Achsen definieren
    switch (ui->comboBox_Frame->currentIndex())
    {
       // Oben
        case 0:
           int_AAxis = 0;
           int_CAxis = 0;
           int_ZOffset = -10;
           break;

        //Hinten
        case 1:
           int_AAxis = -90;
           int_CAxis = 0;
           int_ZOffset = 10;
           break;

        //Links
        case 2:
           int_AAxis = -90;
           int_CAxis = 90;
           int_ZOffset = 10;
           break;

        //Vorne
        case 3:
           int_AAxis = -90;
           int_CAxis = 180;
           int_ZOffset = 10;
           break;

        //Rechts
        case 4:
           int_AAxis = -90;
           int_CAxis = -90;
           int_ZOffset = 10;
           break;
    }

    counter = 0;
    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/C_Algin/Header.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    insert_Content();

    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/C_Algin/Body.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    insert_Content();
}

void C_Algin::slot_pix()
{
    ui->label_Pix->show();
    ui->textEdit->hide();
}

void C_Algin::insert_Content()
{
    QString string_Replace;
    QString string_ReplaceX;
    QString string_ReplaceY;
    double double_Temp;

    //Werte aus LineEdit auslesen
    string_ReplaceX = ui->lineEdit_X->text();
    string_ReplaceY = ui->lineEdit_Y->text();

    // Wenn Oben, X, +
    // setzte CYCL998
    // Gehe in X 10 mm ins Minus
    if(ui->comboBox_Messrichtung1->currentIndex() == 0 &&
       ui->comboBox_Messrichtung2->currentIndex() == 0 &&
       ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,0.2,201,1,$Lange$,,,,,1,0,1,)";
           double_Temp = ui->lineEdit_X->text().toDouble();
           double_Temp -= 10;
           string_ReplaceX = QString("%1").arg(double_Temp);
    }

    // Wenn Oben, X, -
    // setzte CYCL998
    // Gehe in X 10 mm ins Plus
    if(ui->comboBox_Messrichtung1->currentIndex() == 1 &&
        ui->comboBox_Messrichtung2->currentIndex() == 0 &&
        ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,0.2,201,2,$Lange$,,,,,1,0,1,)";
           double_Temp = ui->lineEdit_X->text().toDouble();
           double_Temp += 10;
           string_ReplaceX = QString("%1").arg(double_Temp);
    }

    // Wenn Oben, Y, +
    // setzte CYCL998
    // Gehe in Y 10 mm ins Minus
    if(ui->comboBox_Messrichtung1->currentIndex() == 0 &&
        ui->comboBox_Messrichtung2->currentIndex() == 1 &&
        ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,0.2,102,1,$Lange$,,,,,1,0,1,)";
           double_Temp = ui->lineEdit_Y->text().toDouble();
           double_Temp -= 10;
           string_ReplaceY = QString("%1").arg(double_Temp);
    }

    // Wenn Oben, Y, -
    // setzte CYCL998
    // Gehe in Y 10 mm ins Minus
    if(ui->comboBox_Messrichtung1->currentIndex() == 1 &&
        ui->comboBox_Messrichtung2->currentIndex() == 1 &&
        ui->comboBox_Frame->currentIndex() == 0)
    {
           string_CYCLE998 = "CYCLE998(100105,9000,6,1,1,0,,10,0.2,102,2,$Lange$,,,,,1,0,1,)";
           double_Temp = ui->lineEdit_Y->text().toDouble();
           double_Temp += 10;
           string_ReplaceY = QString("%1").arg(double_Temp);
    }

    if(int_ZOffset >= 0)
           string_Replace = QString("+%1").arg(int_ZOffset);
    else
           string_Replace = QString("%1").arg(int_ZOffset);


    foreach (QString str, stringList_Content)
    {
       if(str.startsWith("#"))
         continue;

       counter++;
       //Header
       str = str.replace("$A$", QString("%1").arg(int_AAxis));
       str = str.replace("$C$", QString("%1").arg(int_CAxis));

       //Body
       str = str.replace("$X$", string_ReplaceX);
       str = str.replace("$Y$", string_ReplaceY);
       str = str.replace("$Z$", ui->lineEdit_Z->text());
       str = str.replace("$ZO$", string_Replace);
       str = str.replace("$CYCLE998$", string_CYCLE998);
       str = str.replace("$Lange$", QString("%1").arg(ui->spinBox_L2->value()));

       str = QString("N%1 ").arg(counter) + str;
       ui->textEdit->append(str);
    }
}

QString C_Algin::replace_Comma(QString str)
{
    QString returnString;
    returnString = str.replace(",",".");
    return returnString;
}
