#include "dialogStart.h"
#include "ui_dialogStart.h"

DialogStart::DialogStart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStart)
{
    ui->setupUi(this);

    paletteInValid = new QPalette();
    paletteInValid->setColor(QPalette::Text,Qt::red);

    paletteValid = new QPalette();
    paletteValid->setColor(QPalette::Text,Qt::black);

    QVBoxLayout *centralLayout = new QVBoxLayout(this);
    centralLayout->addWidget(ui->label_Projektname);

    QHBoxLayout* hboxlayout_Project = new QHBoxLayout(this);
    hboxlayout_Project->addWidget(ui->lineEdit_Projektname);
    hboxlayout_Project->addWidget(ui->lineEdit_ProjektStand);
    centralLayout->addLayout(hboxlayout_Project);

    QHBoxLayout* hboxlayout_Spannung = new QHBoxLayout(this);
    hboxlayout_Spannung->addWidget(ui->radioButton_Spannung1);
    hboxlayout_Spannung->addWidget(ui->radioButton_Spannung2);
    hboxlayout_Spannung->addWidget(ui->radioButton_Spannung3);
    centralLayout->addLayout(hboxlayout_Spannung);


    QVBoxLayout* vboxlayout_Rohteil = new QVBoxLayout(this);
    vboxlayout_Rohteil->addWidget(ui->label_Rohteil);


    QHBoxLayout* hboxlayout_XYZ = new QHBoxLayout(this);
    hboxlayout_XYZ->addWidget(ui->label_X);
    hboxlayout_XYZ->addWidget(ui->lineEdit_X);
    hboxlayout_XYZ->addWidget(ui->label_Y);
    hboxlayout_XYZ->addWidget(ui->lineEdit_Y);
    hboxlayout_XYZ->addWidget(ui->label_Z);
    hboxlayout_XYZ->addWidget(ui->lineEdit_Z);
    vboxlayout_Rohteil->addLayout(hboxlayout_XYZ);
    centralLayout->addLayout(vboxlayout_Rohteil);

    QHBoxLayout* hboxlayout_ZRohteil = new QHBoxLayout(this);
    hboxlayout_ZRohteil->addWidget(ui->label_ZRohTeil);
    hboxlayout_ZRohteil->addWidget(ui->doubleSpinBox_ZRohTeil);
    centralLayout->addLayout(hboxlayout_ZRohteil);

    centralLayout->addWidget(ui->comboBox_Material);
    ui->comboBox_Material->addItem("*");
    readMaterial();
    ui->comboBox_Material->addItems(stringList_Material);

    centralLayout->addWidget(ui->buttonBox);


    ui->radioButton_Spannung1->setChecked(true);
    lineEdit_Projekt =  ui->lineEdit_Projektname;
    lineEdit_ProjektStand = ui->lineEdit_ProjektStand;
    lineEdit_RohteilX = ui->lineEdit_X;
    lineEdit_RohteilY = ui->lineEdit_Y;
    lineEdit_RohteilZ = ui->lineEdit_Z;

    radioButton_Sp1 = ui->radioButton_Spannung1;
    radioButton_Sp2 = ui->radioButton_Spannung2;

    comboBox_Material =  ui->comboBox_Material;

    doubleSpinBox_ZRohTeil = ui->doubleSpinBox_ZRohTeil;

    //checkBox_WriteG55 = ui->checkBox_WriteG55;

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slot_CheckInput()));
    connect(ui->radioButton_Spannung1, SIGNAL(toggled(bool)), this, SLOT(slot_Sp1_Toggled(bool)));
}

DialogStart::~DialogStart()
{
    delete ui;
}

void DialogStart::loadProjectName()
{
    qDebug() << Q_FUNC_INFO;

    QDir dir;
    QStringList filters;
    QStringList stringList_Prg;
    QStringList stringList_TMP;
    QString string_Projektname;
    QStringList stringList_Projektnames;

    bool bool_first = true;
    filters << "*.spf";
    dir.setNameFilters(filters);

    /* Erstellt ein Verzeichnis in dem die CNC-Programme sind
     * lädt alle CNC-Prammnamen in die stringList_Prg */
    dir.setPath(settings->value("ProgrammDir", "").toString());
    stringList_Prg = dir.entryList(QDir::Files);

    /* Lösche alle Einträge in der StringList Projektnames */
    stringList_Projektnames.clear();

    /* Geh durch alle gefunden SPF-Dateien und such nach einen Projektnamen
     * Wenn es die erste SPF-Datei ist schreibe den Projektnamen in die Liste
     * ansonsten sieh in der Liste nach ob es den aktuellen Prjektnamen schon gibt
     * Wenn nicht sende das Signal differentProjectFound*/
    foreach (QString string_Prg, stringList_Prg)
    {
        qDebug() << string_Prg << ": " << string_Projektname;
        string_Projektname = parser_Programm->parseProjectName(dir.path() + "/" + string_Prg);
        if(bool_first)
        {
           stringList_Projektnames.append(string_Projektname);
           bool_first = false;
           continue;
        }

        if(!stringList_Projektnames.contains(string_Projektname))
        {
           emit sig_DifferentProjectFound();
           this->close();
           return;
        }
    }

    /* Wenn die stringList_Prg Einträge hat, also wenn CNC-Programme
     * vorhanden sind, lade mir aus dem ersten CNC-Programm den Projektnamen */
    if(!stringList_Prg.isEmpty())
    {
        qDebug() << "if(!stringList_Prg.isEmpty())";
        //string_Projektname = "nichts";
        /* Zieh den ProjektNamen und ProjektStand aus dem CNC-Programm */
        string_Projektname = parser_Programm->parseProjectName(dir.path() + "/" + stringList_Prg.first());

        /* Ersetze alle Leerzeichen durch einen Unterstrich */
        string_Projektname = string_Projektname.replace(" ", "_");

        /* Splitte den String nach "_"
         * Erster Teil ist der ProjektName
         * Zweiter Teil ist der ProjektStand */
        if(!string_Projektname.isEmpty())
        {
            stringList_TMP = string_Projektname.split("_");
            if(stringList_TMP.size()>0)
                ui->lineEdit_Projektname->setText(stringList_TMP.first());
            if(stringList_TMP.size()>1)
                ui->lineEdit_ProjektStand->setText(stringList_TMP.at(1));
            if(stringList_TMP.size()>2)
            {
                string_SPx = stringList_TMP.at(2);
                if(string_SPx == "SP2" ||
                   string_SPx == "Sp2" ||
                   string_SPx == "sp2")

                    ui->radioButton_Spannung2->setChecked(true);
            }
        }

    }
}

void DialogStart::showDialog()
{
    if(firstStart)
        this->show();

    firstStart = false;
}

void DialogStart::slot_CheckInput()
{
    QString string_ProjektName;
    QString string_ProjektStand;

    bool bool_valid = true;
    ui->lineEdit_Projektname->setPalette(*paletteValid);
    ui->lineEdit_ProjektStand->setPalette(*paletteValid);

    if(ui->lineEdit_Projektname->text().isEmpty() ||
       ui->lineEdit_Projektname->text() == "*")
    {
        ui->lineEdit_Projektname->setPalette(*paletteInValid);
        ui->lineEdit_Projektname->setText("*");
        bool_valid = false;
    }

    if(ui->lineEdit_ProjektStand->text().isEmpty() ||
       ui->lineEdit_ProjektStand->text() == "*")
    {
        ui->lineEdit_ProjektStand->setPalette(*paletteInValid);
        ui->lineEdit_ProjektStand->setText("*");
        bool_valid = false;
    }

    if((ui->lineEdit_X->text().isEmpty() ||
        ui->lineEdit_X->text() == "*" )  &&
       ui->radioButton_Spannung1->isChecked())
    {
        ui->lineEdit_X->setPalette(*paletteInValid);
        ui->lineEdit_X->setText("*");
        bool_valid = false;
    }

    if((ui->lineEdit_Y->text().isEmpty() ||
        ui->lineEdit_Y->text() == "*" )  &&
       ui->radioButton_Spannung1->isChecked())
    {
        ui->lineEdit_Y->setPalette(*paletteInValid);
        ui->lineEdit_Y->setText("*");
        bool_valid = false;

    }

    if((ui->lineEdit_Z->text().isEmpty() ||
        ui->lineEdit_Z->text() == "*" )  &&
       ui->radioButton_Spannung1->isChecked())
    {
        ui->lineEdit_Z->setPalette(*paletteInValid);
        ui->lineEdit_Z->setText("*");
        bool_valid = false;
    }

    if(ui->comboBox_Material->currentText() == "*")
    {
        ui->comboBox_Material->setPalette(*paletteInValid);
        ui->comboBox_Material->setCurrentText("*");
        bool_valid = false;
    }
    if(!bool_valid)
    {
        this->show();
        return;
    }

    string_ProjektName = ui->lineEdit_Projektname->text();
    string_ProjektName = string_ProjektName.replace(" ", "_");
    ui->lineEdit_Projektname->setText(string_ProjektName);

    string_ProjektStand = ui->lineEdit_ProjektStand->text();
    string_ProjektStand = string_ProjektStand.replace(" ", "_");
    ui->lineEdit_ProjektStand->setText(string_ProjektStand);

    emit allValid();
}

void DialogStart::slot_Sp1_Toggled(bool b)
{
       ui->lineEdit_X->setEnabled(b);
       ui->lineEdit_Y->setEnabled(b);
       ui->lineEdit_Z->setEnabled(b);
       //ui->checkBox_WriteG55->setEnabled(b);
       ui->label_Rohteil->setEnabled(b);
       ui->label_X->setEnabled(b);
       ui->label_Y->setEnabled(b);
       ui->label_Z->setEnabled(b);
       ui->doubleSpinBox_ZRohTeil->setEnabled(b);
       ui->label_ZRohTeil->setEnabled(b);
}

void DialogStart::readMaterial()
{
    QFile file(QDir::homePath() + "/MainGen/config/Material.txt");
    QTextStream in(&file);
    QString string_Line;

    if(!file.exists())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") +QDir::homePath() + QString("/MainGen/config/Material.txt ist nicht vorhanden"));
        //qDebug() << QString(Q_FUNC_INFO + QString(" - Datei: ") +QDir::homePath() + QString("/MainGen/config/Material.txt ist nicht vorhanden"));
        return;
    }

    if(!file.open(QFile::ReadOnly))
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") +QDir::homePath() + QString("/MainGen/config/Material.txt konnte nicht geöffnet werden"));
        //qDebug() << QString(Q_FUNC_INFO + QString(" - Datei: ") +QDir::homePath() + QString("/MainGen/config/Material.txt konnte nicht geöffnet werden"));
        return;
    }

    while (!in.atEnd())
    {
        string_Line = in.readLine();
        if(string_Line.startsWith("#"))
            continue;

        stringList_Material.append(string_Line);
    }
    file.close();
}
