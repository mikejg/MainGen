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
    *paletteValid = palette();

    mfile_ZeroPoint = new MFile(QDir::homePath() + "/MainGen/config/Nullpunkt.txt");
    mfile_Material  = new MFile(QDir::homePath() + "/MainGen/config/Material.txt");

    ui->comboBox_Material->addItem("*");

    if(ui->checkBox_RT_EinzelAufmass->isChecked())
    {
        ui->widget_RT_AufmassEinzel->show();
    }
    else
    {
        ui->widget_RT_AufmassEinzel->hide();
    }

    if(ui->checkBox_FT_EinzelAufmass->isChecked())
    {
        ui->widget_FT_AufmassEinzel->show();
    }
    else
    {
        ui->widget_FT_AufmassEinzel->hide();
    }

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slot_CheckInput()));
    connect(ui->checkBox_RT_EinzelAufmass,SIGNAL(stateChanged(int)), this, SLOT(slot_checkBox_RT_AufmassEinzel_stateChanged(int)));
    connect(ui->checkBox_FT_EinzelAufmass,SIGNAL(stateChanged(int)), this, SLOT(slot_checkBox_FT_AufmassEinzel_stateChanged(int)));
    connect(ui->comboBox_Clamping, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBox_NP_indexChanged(int)));

    connect(mfile_Material,  SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(mfile_Material,  SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile_ZeroPoint, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(mfile_ZeroPoint, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
}

DialogStart::~DialogStart()
{
    delete ui;
}

void DialogStart::slot_CheckInput()
{
    //qDebug() << Q_FUNC_INFO;
    QString string_ProjectName;
    QString string_ProjectStatus;
    QString string_Komma;
    bool bool_OK;
    bool bool_valid = true;

    ui->lineEdit_ProjectName->setPalette(*paletteValid);
    ui->lineEdit_ProjectStatus->setPalette(*paletteValid);

    if(ui->lineEdit_ProjectName->text().isEmpty() ||
        ui->lineEdit_ProjectName->text() == "*")
    {
        ui->lineEdit_ProjectName->setPalette(*paletteInValid);
        ui->lineEdit_ProjectName->setText("*");
        bool_valid = false;
    }

    if(ui->lineEdit_ProjectStatus->text().isEmpty() ||
        ui->lineEdit_ProjectStatus->text() == "*")
    {
        ui->lineEdit_ProjectStatus->setPalette(*paletteInValid);
        ui->lineEdit_ProjectStatus->setText("*");
        bool_valid = false;
    }

    if(ui->comboBox_Clamping->currentIndex() == 1)
    {
        //Checke ob in den Eingabefelder für das Rohteil alle Angaben vollständig sind
        //Checke ob in den Eingabefelder für das Bauteil alle Angaben vollständig sind
        if(ui->lineEdit_RTx->text().isEmpty() || ui->lineEdit_RTx->text() == "*" )
        {
            ui->lineEdit_RTx->setPalette(*paletteInValid);
            ui->lineEdit_RTx->setText("*");
            bool_valid = false;
        }

        if(ui->lineEdit_RTy->text().isEmpty() || ui->lineEdit_RTy->text() == "*" )
        {
            ui->lineEdit_RTy->setPalette(*paletteInValid);
            ui->lineEdit_RTy->setText("*");
            bool_valid = false;
        }

        if(ui->lineEdit_RTz->text().isEmpty() || ui->lineEdit_RTz->text() == "*" )
        {
            ui->lineEdit_RTz->setPalette(*paletteInValid);
            ui->lineEdit_RTz->setText("*");
            bool_valid = false;
        }

        if(ui->lineEdit_BTx->text().isEmpty() || ui->lineEdit_BTx->text() == "*" )
        {
            ui->lineEdit_BTx->setPalette(*paletteInValid);
            ui->lineEdit_BTx->setText("*");
            bool_valid = false;
        }

        if(ui->lineEdit_BTy->text().isEmpty() || ui->lineEdit_BTy->text() == "*" )
        {
            ui->lineEdit_BTy->setPalette(*paletteInValid);
            ui->lineEdit_BTy->setText("*");
            bool_valid = false;
        }

        if(ui->lineEdit_BTz->text().isEmpty() || ui->lineEdit_BTz->text() == "*" )
        {
            ui->lineEdit_BTz->setPalette(*paletteInValid);
            ui->lineEdit_BTz->setText("*");
            bool_valid = false;
        }
    }

    if(ui->comboBox_Clamping->currentIndex() == 1)
    {
        // Komma in Pnkut umwandeln
        string_Komma = ui->lineEdit_BTx->text();
        string_Komma = string_Komma.replace(",", ".");
        ui->lineEdit_BTx->setText(string_Komma);

        string_Komma = ui->lineEdit_BTy->text();
        string_Komma = string_Komma.replace(",", ".");
        ui->lineEdit_BTy->setText(string_Komma);

        string_Komma = ui->lineEdit_BTz->text();
        string_Komma = string_Komma.replace(",", ".");
        ui->lineEdit_BTz->setText(string_Komma);

        string_Komma = ui->lineEdit_RTx->text();
        string_Komma = string_Komma.replace(",", ".");
        ui->lineEdit_RTx->setText(string_Komma);

        string_Komma = ui->lineEdit_RTy->text();
        string_Komma = string_Komma.replace(",", ".");
        ui->lineEdit_RTy->setText(string_Komma);

        string_Komma = ui->lineEdit_RTz->text();
        string_Komma = string_Komma.replace(",", ".");
        ui->lineEdit_RTz->setText(string_Komma);


        ui->lineEdit_BTx->text().toDouble(&bool_OK);
        if(!bool_OK)
        {
            ui->lineEdit_BTx->setPalette(*paletteInValid);
            bool_valid = false;
        }
        ui->lineEdit_BTy->text().toDouble(&bool_OK);
        if(!bool_OK)
        {
            ui->lineEdit_BTy->setPalette(*paletteInValid);
            bool_valid = false;
        }
        ui->lineEdit_BTz->text().toDouble(&bool_OK);
        if(!bool_OK)
        {
            ui->lineEdit_BTz->setPalette(*paletteInValid);
            bool_valid = false;
        }

        ui->lineEdit_RTx->text().toDouble(&bool_OK);
        if(!bool_OK)
        {
            ui->lineEdit_RTx->setPalette(*paletteInValid);
            bool_valid = false;
        }
        ui->lineEdit_RTy->text().toDouble(&bool_OK);
        if(!bool_OK)
        {
            ui->lineEdit_RTy->setPalette(*paletteInValid);
            bool_valid = false;
        }
        ui->lineEdit_RTz->text().toDouble(&bool_OK);
        if(!bool_OK)
        {
            ui->lineEdit_RTz->setPalette(*paletteInValid);
            bool_valid = false;
        }
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

    string_ProjectName = ui->lineEdit_ProjectName->text();
    string_ProjectName = string_ProjectName.replace(" ", "_");
    ui->lineEdit_ProjectName->setText(string_ProjectName);

    string_ProjectStatus = ui->lineEdit_ProjectStatus->text();
    string_ProjectStatus = string_ProjectStatus.replace(" ", "_");
    ui->lineEdit_ProjectStatus->setText(string_ProjectStatus);

    project->set_ProjectName(ui->lineEdit_ProjectName->text());
    project->set_ProjectStatus(ui->lineEdit_ProjectStatus->text());

    project->set_BauteilX(ui->lineEdit_BTx->text());
    project->set_BauteilY(ui->lineEdit_BTy->text());
    project->set_BauteilZ(ui->lineEdit_BTz->text());

    project->set_RohteilX(ui->lineEdit_RTx->text());
    project->set_RohteilY(ui->lineEdit_RTy->text());
    project->set_RohteilZ(ui->lineEdit_RTz->text());


    project->set_ZRohteil(QString("%1").arg(ui->doubleSpinBox_ZRohTeil->value()));

    switch (ui->comboBox_Clamping->currentIndex())
    {
        case 0:
        project->set_ProjectClamping("Sp0");
        project->set_MainProgramm(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP0.MPF");
        break;
        case 1:
        project->set_ProjectClamping("Sp1");
        project->set_MainProgramm(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP1.MPF");
        break;
        case 2:
        project->set_ProjectClamping("Sp2");
        project->set_MainProgramm(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP2.MPF");
        break;
        case 3:
        project->set_ProjectClamping("Sp3");
        project->set_MainProgramm(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP3.MPF");
        break;
        case 4:
        project->set_ProjectClamping("Sp4");
        project->set_MainProgramm(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP4.MPF");
        break;
        case 5:
        project->set_ProjectClamping("Sp5");
        project->set_MainProgramm(QDir::homePath() + "/MainGen/Vorlagen/Hauptprogramm_SP5.MPF");
        break;
    default:
        break;
    }

    project->set_ProjectFullName();
    project->set_RepetitiveManufacturing();

    /* Wenn das maximale Aufmass vom Rohteil definert wurde rechne das Aufmass auf das Bauteil um
       Ansonsten übernimm die Werte aus den SpinBoxen*/
    if(ui->groupBox_AufmassMaxRT->title().contains("Rohteil"))
    {
        double double_AufmassX = (ui->lineEdit_RTx->text().toDouble() -
                                  ui->lineEdit_BTx->text().toDouble()) /2;
        double double_AufmassY = (ui->lineEdit_RTy->text().toDouble() -
                                  ui->lineEdit_BTy->text().toDouble()) /2;

        project->set_XPlus_Max(QString("%1").arg(ui->doubleSpinBox_Xplus_Max->value() + double_AufmassX));
        project->set_XMinus_Max(QString("%1").arg(ui->doubleSpinBox_Xminus_Max->value() + double_AufmassX));
        project->set_YPlus_Max(QString("%1").arg(ui->doubleSpinBox_Yplus_Max->value() + double_AufmassY));
        project->set_YMinus_Max(QString("%1").arg(ui->doubleSpinBox_Yminus_Max->value() + double_AufmassY));
        project->set_ZPlus_Max(QString("%1").arg(ui->doubleSpinBox_Zplus_Max->value() +
                                                 ui->doubleSpinBox_ZRohTeil->value()));
    }
    else
    {
        project->set_XPlus_Max(QString("%1").arg(ui->doubleSpinBox_Xplus_Max->value()));
        project->set_XMinus_Max(QString("%1").arg(ui->doubleSpinBox_Xminus_Max->value()));
        project->set_YPlus_Max(QString("%1").arg(ui->doubleSpinBox_Yplus_Max->value()));
        project->set_YMinus_Max(QString("%1").arg(ui->doubleSpinBox_Yminus_Max->value()));
        project->set_ZPlus_Max(QString("%1").arg(ui->doubleSpinBox_Zplus_Max->value() +
                                                 ui->doubleSpinBox_ZRohTeil->value()));
    }

    project->set_XPlus_Min(QString("%1").arg(ui->doubleSpinBox_Xplus_Min->value()));
    project->set_XMinus_Min(QString("%1").arg(ui->doubleSpinBox_Xminus_Min->value()));
    project->set_YPlus_Min(QString("%1").arg(ui->doubleSpinBox_Yplus_Min->value()));
    project->set_YMinus_Min(QString("%1").arg(ui->doubleSpinBox_Yminus_Min->value()));
    project->set_ZPlus_Min(QString("%1").arg(ui->doubleSpinBox_Zplus_Min->value()));

    project->set_Material(ui->comboBox_Material->currentText());
    project->logProjectData();

    //qDebug() << "Emit allValid";
    emit allValid();
}

void DialogStart::slot_checkBox_RT_AufmassEinzel_stateChanged(int state)
{
    Q_UNUSED(state);
    if(ui->checkBox_RT_EinzelAufmass->isChecked())
    {
        ui->widget_RT_AufmassEinzel->show();
    }
    else
    {
        ui->widget_RT_AufmassEinzel->hide();
    }
}

void DialogStart::slot_checkBox_FT_AufmassEinzel_stateChanged(int state)
{
    Q_UNUSED(state);
    if(ui->checkBox_FT_EinzelAufmass->isChecked())
    {
        ui->widget_FT_AufmassEinzel->show();
    }
    else
    {
        ui->widget_FT_AufmassEinzel->hide();
    }
}

void DialogStart::setProject(Project* p)
{
    project = p;

    ui->lineEdit_ProjectName->setText(project->get_ProjectName());
    ui->lineEdit_ProjectStatus->setText(project->get_ProjectStatus());

    if(project->get_AufmassMax())
    {
        ui->groupBox_AufmassMaxRT->setTitle("Aufmass Max vom Bauteil");
    }
}

bool DialogStart::load_Material()
{
    //Einlesen der Materialien
    if(!mfile_Material->read_Content())
        return false;
    ui->comboBox_Material->addItems(mfile_Material->get_Content());

    return true;
}

bool DialogStart::load_ZeroPoint()
{
    QStringList stringList_Split;
    QString string_Clamping = project->get_ProjectClamping();

    //Einlesen der Nullpunkte
    if(!mfile_ZeroPoint->read_Content())
        return false;
    stringList_ZeroPoint = mfile_ZeroPoint->get_Content();

    foreach(string_Line,stringList_ZeroPoint)
    {
        if(string_Line.contains(" || "))
        {
           stringList_Split = string_Line.split(" || ");
           if(stringList_Split.size()>1)
           {
               map_NP.insert(stringList_Split.at(0), stringList_Split.at(1));
           }
        }
    }

    stringList_ZeroPoint.clear();
    QMapIterator<QString, QString> i(map_NP);
    while (i.hasNext())
    {
        i.next();
        stringList_ZeroPoint.append(i.key());
    }

    ui->comboBox_Clamping->addItems(stringList_ZeroPoint);
     list_Keys = map_NP.keys();

     if(string_Clamping == "SP0" || string_Clamping == "Sp0" || string_Clamping == "sp0")
     {
        ui->comboBox_Clamping->setCurrentText(list_Keys[0]);
        ui->lineEdit_NP->setText(map_NP[list_Keys[0]]);
     }

     if(string_Clamping == "SP1" || string_Clamping == "Sp1" || string_Clamping == "sp1")
     {
        ui->comboBox_Clamping->setCurrentText(list_Keys[1]);
        ui->lineEdit_NP->setText(map_NP[list_Keys[1]]);
     }

     if(string_Clamping == "SP2" || string_Clamping == "Sp2" || string_Clamping == "sp2")
     {
        ui->comboBox_Clamping->setCurrentText(list_Keys[2]);
        ui->lineEdit_NP->setText(map_NP[list_Keys[2]]);
     }

     if(string_Clamping == "SP3" || string_Clamping == "Sp3" || string_Clamping == "sp3")
     {
        ui->comboBox_Clamping->setCurrentText(list_Keys[3]);
        ui->lineEdit_NP->setText(map_NP[list_Keys[3]]);
     }

    return true;
}

void DialogStart::slot_comboBox_NP_indexChanged(int i)
{
    if(i < list_Keys.size())
        ui->lineEdit_NP->setText(map_NP[list_Keys[i]]);
}
