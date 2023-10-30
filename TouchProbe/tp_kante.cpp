#include "tp_kante.h"
#include "ui_tp_kante.h"

TP_Kante::TP_Kante(QWidget *parent) :
    TP_Base(parent),
    ui(new Ui::TP_Kante)
{
    ui->setupUi(this);
    ui->lineEdit_Wert->installEventFilter(this);

    foreach(QString str, stringList_ContentAnfahren)
    {
        ui->textEdit_Anfahren->append(str);
    }

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

    return false;
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
}

void TP_Kante::slot_SignsChanged(QString str)
{

}
