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
    *paletteValid = palette();

    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setFilter(QDir::Dirs);

    connect(ui->buttonBox,             SIGNAL(accepted()),    this, SLOT(writeSettings()));
    connect(ui->toolButton_Programme,  SIGNAL(clicked(bool)), this, SLOT(toolButton_Programme_clicked(bool)));
    connect(ui->toolButton_Magazin,    SIGNAL(clicked(bool)), this, SLOT(toolButton_Magazin_clicked(bool)));
    connect(ui->toolButton_WerkzeugDB, SIGNAL(clicked(bool)), this, SLOT(toolButton_WerkzeugDB_clicked(bool)));
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::set_Settings(Settings* s)
{
    settings = s;

    ui->lineEdit_Programme->setText(s->get_ProgrammDir());
    ui->lineEdit_Magazin->setText(s->get_MagazinDir());
    ui->lineEdit_WerkzeugDB->setText(s->get_WerkzeugDB());
    ui->checkBox_Numbering->setChecked(s->get_Numbering());
    ui->spinBox_WerkzeugPlatze->setValue(s->get_WerkzeugPlatze());
    ui->checkBox_AufmassMax->setChecked(s->get_AufmassMax());
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

    file.setFileName(dir.path() + "/Magazin.INI");
    if (!file.exists())
    {
        emit sig_Err("File " + dir.path() + "/Magazin.INI" +
                     " konnte nicht gefunden werden");
        bool_Return = false;
    }
    file.setFileName(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP0.MPF");
    if (!file.exists())
    {
        emit sig_Err("File " + QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP0.MPF" +
                     " konnte nicht gefunden werden");
        bool_Return = false;
    }

    file.setFileName(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP1.MPF");
    if (!file.exists())
    {
        emit sig_Err("File " + QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP1.MPF" +
                     " konnte nicht gefunden werden");
        bool_Return = false;
    }

    file.setFileName(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP2.MPF");
    if (!file.exists())
    {
        emit sig_Err("File " + QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP2.MPF" +
                     " konnte nicht gefunden werden");
        bool_Return = false;
    }

    file.setFileName(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP3.MPF");
    if (!file.exists())
    {
        emit sig_Err("File " + QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP3.MPF" +
                     " konnte nicht gefunden werden");
    }

    file.setFileName(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP4.MPF");
    if (!file.exists())
    {
        emit sig_Err("File " + QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP4.MPF" +
                     " konnte nicht gefunden werden");
        bool_Return = false;
    }

    file.setFileName(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP5.MPF");
    if (!file.exists())
    {
        emit sig_Err("File " + QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP5.MPF" +
                     " konnte nicht gefunden werden");
        bool_Return = false;
    }

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
 bool bool_Numbering = ui->checkBox_Numbering->isChecked();
 bool bool_AufmassMax = ui->checkBox_AufmassMax->isChecked();
 settings->set_ProgrammDir(ui->lineEdit_Programme->text());
 settings->set_MagazinDir(ui->lineEdit_Magazin->text());
 settings->set_WerkzeugDB(ui->lineEdit_WerkzeugDB->text());
 settings->set_Numbering(QVariant(bool_Numbering).toString());
 settings->set_WerkzeugPlatze(ui->spinBox_WerkzeugPlatze->value());
 settings->set_AufmassMax(QVariant(bool_AufmassMax).toString());
 emit settingsOK();
}

void DialogSettings::toolButton_Programme_clicked(bool b)
{
 Q_UNUSED(b);
 //qDebug() << "toolButton_Programme_clicked";
 ui->lineEdit_Programme->setPalette(*paletteValid);
 ui->lineEdit_Programme->setText( fileDialog->getExistingDirectory(this,tr("Verzeichnis Programme"), ""));
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
