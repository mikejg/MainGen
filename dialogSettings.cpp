#include "dialogSettings.h"
#include "ui_dialogSettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    paletteInValid = new QPalette();
    paletteInValid->setColor(QPalette::Text,Qt::red);

    paletteValid = new QPalette();
    paletteValid->setColor(QPalette::Text,Qt::black);

    QVBoxLayout *centralLayout = new QVBoxLayout(this);

    centralLayout->addWidget(ui->label_Programme);
    QHBoxLayout *programmeLayout = new QHBoxLayout(this);
    programmeLayout->addWidget(ui->lineEdit_Programme);
    programmeLayout->addWidget(ui->toolButton_Programme);
    centralLayout->addLayout(programmeLayout);

    centralLayout->addWidget(ui->label_Spannung1);
    QHBoxLayout* spannung1Layout = new QHBoxLayout(this);
    spannung1Layout->addWidget(ui->lineEdit_Spannung1);
    spannung1Layout->addWidget(ui->toolButton_Spannung1);
    centralLayout->addLayout(spannung1Layout);

    centralLayout->addWidget(ui->label_Spannung2);
    QHBoxLayout* spannung2Layout = new QHBoxLayout(this);
    spannung2Layout->addWidget(ui->lineEdit_Spannung2);
    spannung2Layout->addWidget(ui->toolButton_Spannung2);
    centralLayout->addLayout(spannung2Layout);

    centralLayout->addWidget(ui->label_Magazin);
    QHBoxLayout* magazinLayout = new QHBoxLayout(this);
    magazinLayout->addWidget(ui->lineEdit_Magazin);
    magazinLayout->addWidget(ui->toolButton_Magazin);
    centralLayout->addLayout(magazinLayout);

    centralLayout->addWidget(ui->label_WerkzeugDB);
    QHBoxLayout* werkzeugDBLayout = new QHBoxLayout(this);
    werkzeugDBLayout->addWidget(ui->lineEdit_WerkzeugDB);
    werkzeugDBLayout->addWidget(ui->toolButton_WerkzeugDB);
    centralLayout->addLayout(werkzeugDBLayout);

    centralLayout->addWidget(ui->buttonBox);

    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setFilter(QDir::Dirs);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(writeSettings()));
    connect(ui->toolButton_Programme, SIGNAL(clicked(bool)), this, SLOT(toolButton_Programme_clicked(bool)));
    connect(ui->toolButton_Spannung1, SIGNAL(clicked(bool)), this, SLOT(toolButton_VorlageSp1_clicked(bool)));
    connect(ui->toolButton_Spannung2, SIGNAL(clicked(bool)), this, SLOT(toolButton_VorlageSp2_clicked(bool)));
    connect(ui->toolButton_Magazin, SIGNAL(clicked(bool)), this, SLOT(toolButton_Magazin_clicked(bool)));
    connect(ui->toolButton_WerkzeugDB, SIGNAL(clicked(bool)), this, SLOT(toolButton_WerkzeugDB_clicked(bool)));
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

bool DialogSettings::checkSettings()
{
    bool bool_Return = true;
    QFile file;
    QDir dir;

    dir = QDir(ui->lineEdit_Programme->text());
    if (!dir.exists())
    {
        ui->lineEdit_Programme->setPalette(*paletteInValid);
        bool_Return = false;
    }
    else
        ui->lineEdit_Programme->setPalette(*paletteValid);

    dir = QDir(ui->lineEdit_Magazin->text());
    if (!dir.exists())
    {
        ui->lineEdit_Magazin->setPalette(*paletteInValid);
        bool_Return = false;
    }
    else
        ui->lineEdit_Magazin->setPalette(*paletteValid);

    file.setFileName(ui->lineEdit_Spannung1->text());
    if (!file.exists())
    {
        ui->lineEdit_Spannung1->setPalette(*paletteInValid);
        bool_Return = false;
    }
    else
        ui->lineEdit_Spannung1->setPalette(*paletteValid);

    file.setFileName(ui->lineEdit_Spannung2->text());
    if (!file.exists())
    {
        ui->lineEdit_Spannung2->setPalette(*paletteInValid);
        bool_Return = false;
    }
    else
        ui->lineEdit_Spannung2->setPalette(*paletteValid);

    file.setFileName(ui->lineEdit_WerkzeugDB->text());
    if (!file.exists())
    {
        ui->lineEdit_WerkzeugDB->setPalette(*paletteInValid);
        bool_Return = false;
    }
    else
        ui->lineEdit_WerkzeugDB->setPalette(*paletteValid);
    return bool_Return;
}


void DialogSettings::writeSettings()
{
 //qDebug() << "Dialog_Settings::writeSettings";
 if(!checkSettings())
 {
     this->show();
     return;
 }
 settings->setValue("ProgrammDir",ui->lineEdit_Programme->text());
 settings->setValue("VorlageSp1",ui->lineEdit_Spannung1->text());
 settings->setValue("VorlageSp2",ui->lineEdit_Spannung2->text());
 settings->setValue("MagazinDir",ui->lineEdit_Magazin->text());
 settings->setValue("WerkzeugDB", ui->lineEdit_WerkzeugDB->text());

 settingsOK();
}

void DialogSettings::toolButton_Programme_clicked(bool b)
{
 Q_UNUSED(b);
 //qDebug() << "toolButton_Programme_clicked";
 ui->lineEdit_Programme->setPalette(*paletteValid);
 ui->lineEdit_Programme->setText( fileDialog->getExistingDirectory(this,tr("Verzeichnis Programme"), ""));
}

void DialogSettings::toolButton_VorlageSp1_clicked(bool b)
{
    Q_UNUSED(b);
    //qDebug() << "toolButton_Spannung1_clicked";
    ui->lineEdit_Spannung1->setPalette(*paletteValid);
    ui->lineEdit_Spannung1->setText( fileDialog->getOpenFileName(this,"Vorlage Spannung1",QDir::homePath(),tr("Main Programm File(*.MPF)")));
}

void DialogSettings::toolButton_VorlageSp2_clicked(bool b)
{
    Q_UNUSED(b);
    //qDebug() << "toolButton_Spannung2_clicked";
    ui->lineEdit_Spannung2->setPalette(*paletteValid);
    ui->lineEdit_Spannung2->setText( fileDialog->getOpenFileName(this,"Vorlage Spannung2",QDir::homePath(),tr("Main Programm File(*.MPF)")));
}

void DialogSettings::toolButton_Magazin_clicked(bool b)
{
    Q_UNUSED(b);
    //qDebug() << "toolButton_Spannung2_clicked";
    ui->lineEdit_Magazin->setPalette(*paletteValid);
    ui->lineEdit_Magazin->setText( fileDialog->getExistingDirectory(this,tr("Verzeichnis Werkzeugmagazin"), ""));
}

void DialogSettings::toolButton_WerkzeugDB_clicked(bool b)
{
    Q_UNUSED(b);
    //qDebug() << "toolButton_Spannung2_clicked";
    ui->lineEdit_WerkzeugDB->setPalette(*paletteValid);
    ui->lineEdit_WerkzeugDB->setText( fileDialog->getOpenFileName(this,"Werkzeug Datenbank",QDir::homePath(),tr("Main Programm File(*.db)")));
}

void DialogSettings::set_ProgrammDir(QString s)
{ui->lineEdit_Programme->setText(s);}

void DialogSettings::set_VorlageSp1(QString s)
{ui->lineEdit_Spannung1->setText(s);}

void DialogSettings::set_VorlageSp2(QString s)
{ui->lineEdit_Spannung2->setText(s);}

void DialogSettings::set_Magazin(QString s)
{ui->lineEdit_Magazin->setText(s);}

void DialogSettings::set_WerkzeugDB(QString s)
{ui->lineEdit_WerkzeugDB->setText(s);}
