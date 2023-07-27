#include "c_algin.h"
#include "ui_c_algin.h"

C_Algin::C_Algin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_Algin)
{
    ui->setupUi(this);
    ui->spinBox_L2->installEventFilter(this);

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

    connect(ui->comboBox_Frame, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentFrameChanged(QString)));
    connect(ui->comboBox_Messrichtung1, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentTextChanged(QString)));
    connect(ui->comboBox_Messrichtung2, SIGNAL(currentTextChanged(QString)), this, SLOT(slot_currentTextChanged(QString)));
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
