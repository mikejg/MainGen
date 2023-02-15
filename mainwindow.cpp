#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QToolBar>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_MainLayout();

    QFont font("Lucida Console");
    textEdit->setFont(font);
    textEdit_Log->setFont(font);

    toolList_Top100 = new ToolList(this);
    toolList_Magazin = new ToolList(this);
    toolList_Project = new ToolList(this);
    toolList_IN      = new ToolList(this);
    toolList_OUT    = new ToolList(this);
    toolList_Table  = new ToolList(this);
    toolList_AllProjects = new ToolList(this);

    dbManager = new DBManager(this);
    //dbmainManager = new dbMainManager(this);

    parser_Magazin  = new Parser_WZM(this);
    parser_Magazin->setDBManager(dbManager);

    parser          = new Parser(this);
    parser->setDBManager(dbManager);
    parser_Programm = new Parser_Programm(this);
    parser_Programm->setTextEdit(textEdit_Log);
    parser_Programm->setDBManager(dbManager);
    parser->setDBManager(dbManager);


    settings = new QSettings("Gareiss", "MainGen");
    //settings->clear();
    string_ProgrammDir = settings->value("ProgrammDir", "").toString();
    string_VorlageSp1 = settings->value("VorlageSp1", "").toString();
    string_VorlageSp2 = settings->value("VorlageSp2", "").toString();
    string_MagazinDir = settings->value("MagazinDir", "").toString();
    string_WerkzeugDB_orginal = settings->value("WerkzeugDB", "").toString();

    dialogSettings = new DialogSettings(this);
    dialogSettings->set_Settings(settings);
    dialogSettings->set_ProgrammDir(string_ProgrammDir);
    dialogSettings->set_VorlageSp1(string_VorlageSp1);
    dialogSettings->set_VorlageSp2(string_VorlageSp2);
    dialogSettings->set_Magazin(string_MagazinDir);
    dialogSettings->set_WerkzeugDB(string_WerkzeugDB_orginal);


    dialogStart = new DialogStart(this);
    dialogStart->setSettings(settings);
    dialogStart->setParser_Programm(parser_Programm);

    file = new QFile();
    fileDialog = new QFileDialog(this);

    dialogRecover = new DialogRecover(this);
    dialogRecover->setDBManager(dbManager);
    dialogRecover->setParser(parser);

    dialogWrite = new DialogWrite(this);
    dialogRepetition = new DialogRepetition(this);

    /* Zum testen der Überprüfung */
    /*QDateTime dateTime = QDateTime::currentDateTime();
    dateTime = dateTime.addDays(-2);
    settings->setValue("NextCheck", dateTime.toString());*/

    //settings->remove("NextCheck");
    license = new License(this);
    license->setSettings(settings);


    connect(dialogStart, SIGNAL(allValid()),this, SLOT(generate_MPF()));
    connect(ui->actionEinstellungen, SIGNAL(triggered(bool)), this, SLOT(slot_ShowSettings(bool)));
    connect(ui->actionSpeichern, SIGNAL(triggered(bool)), this,SLOT(slot_Save(bool)));
    connect(action_Save, SIGNAL(triggered(bool)), this,SLOT(slot_Save(bool)));
    connect(ui->actionDrucken, SIGNAL(triggered(bool)), this, SLOT(slot_Print(bool)));
    connect(action_Print,      SIGNAL(triggered(bool)), this, SLOT(slot_Print(bool)));
    connect(ui->actionSchwester_Projekt_hinzufuegen, SIGNAL(triggered(bool)), this, SLOT(slot_SchwesterProjekt(bool)));
    connect(ui->actionOeffnen, SIGNAL(triggered(bool)), this, SLOT(slot_OpenRPL(bool)));
    connect(ui->actionRecoverDB, SIGNAL(triggered(bool)), this, SLOT(slot_RecoverDB(bool)));
    connect(ui->actionFinish_File, SIGNAL(triggered(bool)), this, SLOT(slot_CheckFiles(bool)));
    connect(action_FinishFile, SIGNAL(triggered(bool)), this,SLOT(slot_CheckFiles(bool)));
    connect(ui->action_Import_Rpl, SIGNAL(triggered(bool)), this, SLOT(slot_Import_Ruestplan(bool)));

    connect(dialogSettings, SIGNAL(settingsOK()), dialogStart, SLOT(showDialog()));
    connect(dialogSettings, SIGNAL(settingsOK()), this, SLOT(slot_CopyWerkzeugDB()));

    connect(dialogRepetition, SIGNAL(accepted()), this, SLOT(slot_RepetitionAccepted()));
    //connect(this, SIGNAL(send_Value(QString, int)), dialogRecover, SLOT(slot_setValue(QString,int)));

    connect(dbManager, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(dbManager, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(dialogWrite, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(dialogWrite, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(dialogStart, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(dialogStart, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(dialogRecover, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(dialogRecover, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(parser, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(parser, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(parser_Programm, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(parser_Programm, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(parser_Magazin, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(parser_Magazin, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(license,SIGNAL(sig_LicenseFailed()), this, SLOT(slot_LicenseFaild()));

    //dialogStart->show();

    if(!dialogSettings->checkSettings())
    {
        dialogSettings->show();
        return;
    }

    dialogStart->show();
    dialogStart->loadProjectName();

    slot_CopyWerkzeugDB();
    dbManager->open();

    slot_Erstelle_Top100();
    erstelle_Magazin();
    //toolList_Top100->sort_Top100();
    //showTop100();
    showMagazin();
    license->checkLicense();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::erstelle_Magazin()
{
    qDebug() << Q_FUNC_INFO << " start";
    //QDir dir = QDir(string_AppDir + "/WZ_Magazin");
    QDir dir = QDir(string_MagazinDir);
    QFileInfoList fileInfoList = dir.entryInfoList();
    QStringList stringList_Files;

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            if(fileInfo.suffix() == "INI")
            {
              stringList_Files.append(fileInfo.absoluteFilePath());
            }
        }
    }

    toolList_Magazin->clear();
    if(!stringList_Files.isEmpty())
        parser_Magazin->parse(stringList_Files.first(), toolList_Magazin);

    toolList_Magazin->sort_ID();
    qDebug() << Q_FUNC_INFO << " end";
}

void MainWindow::erstelle_ToolList()
{
    toolList_Project->clear();
    toolList_AllProjects->clear();

    /* Suche in allen Programmen nach Werkzeugen */
    foreach(QString string_Programm, stringList_Programme)
    {
       parser_Programm->parse(string_ProgrammDir+ "/" + string_Programm, toolList_Project);
    }

    /* Sortiere die ToolList toolList_Project nach der WerkzeugID */
    toolList_Project->sort_ID();
    foreach(Tool* tool, toolList_Project->getList())
        toolList_AllProjects->insert_Tool(tool);
}

void MainWindow::generate_MPF()
{
    string_ProgrammDir  = settings->value("ProgrammDir", "").toString();
    string_VorlageSp1   = settings->value("VorlageSp1", "").toString();
    string_VorlageSp2   = settings->value("VorlageSp2", "").toString();
    string_ProjektName  = dialogStart->lineEdit_Projekt->text();
    string_Projekt      = string_ProjektName;
    string_ProjektStand = dialogStart->lineEdit_ProjektStand->text();
    string_ProjektDir   = string_ProgrammDir + "/" + string_ProjektName + ".WPD";

    //qDebug() << "string_ProjektDir: " << string_ProjektDir;
    if(string_ProjektName.isEmpty())
    {
        dialogStart->show();
        return;
    }
    load_Programme();
    int wf = dbManager->getWiederholFertigung(string_Projekt);
    string_WiederholFertigung = QString("%1").arg(wf);

    if(dialogStart->radioButton_Sp1->isChecked())
    {
        set_Spannung(string_VorlageSp1);
        string_Projekt = string_ProjektName + "_" +  string_ProjektStand + "_Sp1";
        string_Spannung = "Spannung1";
        string_SpX      = "Sp1";
    }

    if(dialogStart->radioButton_Sp2->isChecked())
    {
        set_Spannung(string_VorlageSp2);
        string_Projekt = string_ProjektName + "_" + string_ProjektStand + "_Sp2";
        string_Spannung = "Spannung2";
        string_SpX      = "Sp2";
    }

   // qDebug() << string_Projekt;
    erstelle_ToolList();
    showTable();
}

void MainWindow::load_Programme()
{

    QDir dir;
    QStringList filters;
    QString string_shortName;
    bool bool_OK;
    int int_I;  Q_UNUSED(int_I);

    filters << "*.spf";
    dir.setNameFilters(filters);

    stringList_Programme.clear();

    dir.setPath(string_ProgrammDir);
    if(!dir.exists())
    {
        string_ProgrammDir = fileDialog->getExistingDirectory(this,tr("Verzeichnis Programme"), "");
        dir.setPath(string_ProgrammDir);
        settings->setValue("ProgrammDir",string_ProgrammDir );
    }

    /*Lösche alle einträge in stringList_Programme
     * lade alle ProgrammNamen in stringList_Programme*/
    stringList_Programme.clear();
    stringList_Programme = dir.entryList(QDir::Files);

    /*Erzeuge eine temporäre StringList tmp
     * geh durch stringList_Programme
     * wenn der Eintrag mit 1_ - 9_ stratet setze eine 0 davor
     * schreibe den Eintrag in tmp*/
    QStringList tmp;
    foreach(QString str, stringList_Programme)
    {
        if (str.startsWith("1_") || str.startsWith("2_") || str.startsWith("3_") ||
            str.startsWith("4_") || str.startsWith("5_") || str.startsWith("6_") ||
            str.startsWith("7_") || str.startsWith("8_") || str.startsWith("9_") )
        {
            dir.rename(str, "0"+str);
            str = "0" + str;
        }

        /* Wenn der Dateiname länger als 28 Zeichen hat soll er gekürzt werden
         * 24 Zeichen + ".spf" ergibt 28 Zeichen
         * Schreiben die ersten 24 Zeichen des Strings str in string_shortName
         * Benenne die Datei um
         * Schreibe den kurzen Dateinamen in den String str*/

        if(str.length() > 31)
        {
            string_shortName = str.left(27) + ".spf";
            dir.rename(str, string_shortName);
            str = string_shortName;
        }


        tmp.append(str);

    }

    /* Sortiere tmp
     * Lösche alle Einträge in stringList_Programme
     * geht durch tmp
     * Schreibe alle Einträge von tmp in stringListProgramme*/
    tmp.sort();
    stringList_Programme.clear();
    foreach(QString str, tmp)
    {
        //if(str.startsWith("0"))
        //    str = str.right(str.length()-1);
        string_shortName = str;
        int_I = string_shortName.left(1).toInt(&bool_OK, 10);
        while(bool_OK)
        {
            string_shortName = string_shortName.right(string_shortName.length()-1);
            int_I = string_shortName.left(1).toInt(&bool_OK, 10);
            if(string_shortName.startsWith("_"))
                bool_OK = true;
        }

        //string_shortName = str.right(str.length()-3);
        dir.rename(str, string_shortName);
        stringList_Programme.append(string_shortName);
    }
}

bool MainWindow::openFile(QString fileName)
{
    file->setFileName(fileName);
    if(file->open(QFile::ReadOnly))
        return true;
    else
        return false;
}

void MainWindow::slot_Save(bool b)
{
    Q_UNUSED(b);

    /* Wenn es keine Wiederholfertigung ist schreibe das Projekt in die Datenbank */
    if(string_WiederholFertigung == "0")
    {
        save_MPF(true);
        return;
    }

    dialogRepetition->show();
    return;

}

void MainWindow::slot_RepetitionAccepted()
{
       qDebug() << Q_FUNC_INFO;
}

void MainWindow::save_MPF(bool b)
{
    /*Erzeugt im Ordern Programme den Ordner ProjektName.WPD/SpannungX.WPD
     *      Programme/E123456789.WPD/Spannung1.WPD
     * Erstellt QDir dir_Vorlagen mit dem Pfad, in dem die Vorlagen für die Spannung sind
     * Erstellt QDir dir_Programme mit dem Pfad 'Programme/E123456789.WPD/Spannung1.WPD'
    */

    //Q_UNUSED(b);
    QString dst = string_ProgrammDir + "/" + string_ProjektName + ".WPD/" +
            string_ProjektName+ "_" + string_ProjektStand + "_" + string_SpX  + ".WPD";
    QString src = QDir::homePath() + "/MainGen/Vorlage.WPD/" + string_Spannung + ".WPD";
    QDir dir_Vorlagen(src);
    //QDir dir_Programme(string_ProgrammDir);

    /*Öffnet das File Programme/00_E123456789.MPF"
     * Schreibt alles aus dem Reiter Hauptprgramm in das File*/
    QString str = string_ProgrammDir + "/" + string_Projekt +".MPF";
    QStringList stringList_Lines = textEdit->toPlainText().split("\n");

    QFile file(str);
    if(file.open(QFile::WriteOnly))
    {
       marker = keinMarker;
       QTextStream stream(&file);
       foreach(QString string_Line, stringList_Lines)
       {
           if(string_Line.contains("$KopfStart$"))
           {
               marker = KopfStart;
               continue;
           }

           if(string_Line.contains("$KopfEnd$"))
           {
               marker = KopfEnd;
               continue;
           }

           if(marker == KopfStart)
           {
               string_Line = formatString(string_Line);
           }

           stream << string_Line << "\n";
       }
       file.close();
    }
    /*
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << textEdit->toPlainText();
        file.flush();
        file.close();
    }
    else
    {
        QMessageBox::critical(this, tr("Errore"), tr("Konnte file nicht speichern"));
        return;
    }
    */

    /*Übergibt dem Parser Ruestplaene/E123456789E01_Sp1.rpl und die ToolListe des Projects zum Speichern*/
    parser->save(QDir::homePath()+ "/MainGen/Ruestplaene/" + string_Projekt + "_" + string_WiederholFertigung + ".rpl", toolList_Project);

    /*Übergibt dem DBManager den Projektnamen und die toolList zum Eintragen*/
    dbManager->insertProject(string_Projekt + "_" + string_WiederholFertigung, toolList_Project);


    /*Kopiert den Ordner Vorlagen.WPD/Spannung1.WPD nach
     * Programme/E123456789.WPD/Spannung1.WPD*/
    dir_Vorlagen.mkpath(dst);
    foreach (QString f, dir_Vorlagen.entryList(QDir::Files))
    {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }

    /*Übergibt dem DiealoWrite ein paar Variable*/
    dialogWrite->set_DST(dst);
    dialogWrite->set_SRC(src);
    dialogWrite->set_Programme(stringList_Programme);
    dialogWrite->set_Parser(parser_Programm);
    dialogWrite->set_ProgrammDir(string_ProgrammDir);
    dialogWrite->set_Projekt(string_Projekt);
    dialogWrite->set_Settings(dialogStart);

    /*Starte DialogWrite*/
    dialogWrite->initDialog();
    return;

    //finish_Antasten();
    //finish_Rohteil_Kontrolle();
}

void MainWindow::set_MainLayout()
{
    QToolBar *toolbar = this->addToolBar("main toolbar");
    action_Open = toolbar->addAction(QIcon(":/Icons/Open.svg"),
          "Rüstplan Öffnen");
    action_Save = toolbar->addAction(QIcon(":/Icons/Save.svg"),
          "Rüstplan Speichern");
    action_Print = toolbar->addAction(QIcon(":/Icons/Print.svg"),
          "Rüstplan Drucken");
    action_FinishFile = toolbar->addAction(QIcon(":/Icons/checklist.svg"),
          "Finish einzelne Files");
    QVBoxLayout *vLayoutMain;

    QWidget *Widget1;
    //Layout to the Tab1
    QVBoxLayout *vLayoutTab1;

    //QWidget *Widget2;
    //Layout to the Tab2
    QVBoxLayout *vLayoutTab2;

    QWidget *Widget3;
    //Layout to the Tab3
    QVBoxLayout *vLayoutTab3;

    QWidget *Widget4;
    //Layout to the Tab4
    QVBoxLayout *vLayoutTab4;

    //QWidget *Widget5;
    //Layout to the Tab5
    QVBoxLayout *vLayoutTab5;

    //Set Vertical Box Layout to the main Window (central widget)
    vLayoutMain = new QVBoxLayout(this->centralWidget());
    tabWidget = new QTabWidget(this->centralWidget());

    Widget1 = new QWidget();
    //Set the Vertical Box Layout to the Widget 1 (holds the tab1)
    vLayoutTab1 = new QVBoxLayout(Widget1);
    textEdit = new QTextEdit (Widget1);
    vLayoutTab1->addWidget(textEdit);
    tabWidget->addTab(Widget1, QString("Hauptprogramm"));

    widget_Ruestplan = new QWidget();
    //Set the Vertical Box Layout to the Widget 2 (holds the tab2)
    vLayoutTab2 = new QVBoxLayout(widget_Ruestplan);
    //Table2 = new QTableWidget(5,5,Widget2);
    tableView = new QTableView(widget_Ruestplan);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setStretchLastSection(true);
    vLayoutTab2->addWidget(tableView);
    tabWidget->addTab(widget_Ruestplan, QString("Rüstplan"));

    Widget3 = new QWidget();
    //Set the Vertical Box Layout to the Widget 2 (holds the tab2)
    vLayoutTab3 = new QVBoxLayout(Widget3);
    tableView_Top100 = new QTableView(Widget3);
    tableView_Top100->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView_Top100->horizontalHeader()->setStretchLastSection(true);
    vLayoutTab3->addWidget(tableView_Top100);
    tabWidget->addTab(Widget3, QString("Top100"));

    Widget4 = new QWidget();
    //Set the Vertical Box Layout to the Widget 2 (holds the tab2)
    vLayoutTab4 = new QVBoxLayout(Widget4);
    tableView_Magazin = new QTableView(Widget4);
    tableView_Magazin->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView_Magazin->horizontalHeader()->setStretchLastSection(true);
    vLayoutTab4->addWidget(tableView_Magazin);
    tabWidget->addTab(Widget4, QString("Magazin"));

    widget_Log = new QWidget();
    //Set the Vertical Box Layout to the Widget 1 (holds the tab1)
    vLayoutTab5 = new QVBoxLayout(widget_Log);
    textEdit_Log = new QTextEdit (widget_Log);
    vLayoutTab5->addWidget(textEdit_Log);
    tabWidget->addTab(widget_Log, QString("Logfile"));


    //Adding the Tab widget to the main layout
    vLayoutMain->addWidget(tabWidget);
    tabWidget->setCurrentIndex(0);
}

void MainWindow::set_Spannung(QString string_Vorlage)
{
    QString string_Line;

   if(!openFile(string_Vorlage))
   {
       slot_Err(Q_FUNC_INFO + QString(" - ") + string_Vorlage + QString(" konnte nicht geöffnet werden"));
       return;
   }

   QTextStream in(file);
   //stringList_Lines.clear();
   while (!in.atEnd())
   {
       string_Line = in.readLine();

       if(string_Line.startsWith("#"))
           continue;

       string_Line = string_Line.replace("$ZNr$", string_Projekt);
       string_Line = string_Line.replace("$ZStd$", string_ProjektStand);
       string_Line = string_Line.replace("$RX$", dialogStart->lineEdit_RohteilX->text());
       string_Line = string_Line.replace("$RY$", dialogStart->lineEdit_RohteilY->text());
       string_Line = string_Line.replace("$RZ$", dialogStart->lineEdit_RohteilZ->text());
       string_Line = string_Line.replace("$Ma$", dialogStart->comboBox_Material->currentText());

       if(string_Line.contains("$G55$"))
       {

               writeG55();
               continue;
        }
       textEdit->append(string_Line);
       if(string_Line.contains(";Programme"))
       {
           foreach(QString str, stringList_Programme)
           {
               string_Line = "call \"" + str.left(str.length()-4) + "\"\n";
               textEdit->append(string_Line);
           }
       }
   }
   file->close();
}

void MainWindow::showMagazin()
{
    list_ToolID.clear();
    list_ToolDescription.clear();
    list_ToolGL.clear();
    list_ToolAL.clear();
    list_ToolFL.clear();
    int magazinSize = toolList_Magazin->getSize();

    list_ToolDescription.append(QString("        Magazin %1  von 125 belegt").arg(magazinSize));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    foreach(Tool* tool, toolList_Magazin->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(QString("%1").arg(tool->get_counter()));
    }

    tableModel = new TableModel(this);

    // Populate model with data:
    tableModel->populateData(list_ToolID,
                             list_ToolDescription,
                             list_ToolGL,
                             list_ToolAL,
                             list_ToolFL);

    // Connect model to table view:
    tableView_Magazin->setModel(tableModel);

    // Make table header visible and display table:
    //tableView->horizontalHeader()->setVisible(true);
    tableView_Magazin->show();
}

void MainWindow::showTable()
{
    toolList_IN->clear();
    toolList_OUT->clear();
    toolList_Table->clear();
    int allProjects_Size;
    int in_Size;
    int out_Size;

    foreach(Tool* tool, toolList_AllProjects->getList())
        toolList_Table->insert_Tool(tool);

    toolList_Table->sort_ID();
    allProjects_Size = toolList_AllProjects->getSize();
    //-- ab hier neue Funktion
    //Vergleich Project mit dem Werkzeugmagazin

    foreach(Tool* tool, toolList_Table->getList())
    {
        if(!toolList_Magazin->contains(tool))
            toolList_IN->insert_Tool(tool);
    }
    in_Size = toolList_IN->getSize();

    //Vergleiche Werkzeugmagazin mit Top100 und Projekt
    foreach(Tool* tool, toolList_Magazin->getList())
    {
        if(!toolList_Top100->contains(tool) && !toolList_Table->contains(tool))
            toolList_OUT->insert_Tool(tool);
    }
    toolList_OUT->sort_Counter();

    out_Size = toolList_OUT->getSize();

    //qDebug() << toolList_OUT->getList().size();

    list_ToolID.clear();
    list_ToolDescription.clear();
    list_ToolGL.clear();
    list_ToolAL.clear();
    list_ToolFL.clear();

    list_ToolDescription.append(QString(string_Projekt + "  %1 Werkzeuge").arg(allProjects_Size));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    foreach(Tool* tool, toolList_Table->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(tool->get_ToolFL());
    }

    list_ToolDescription.append(" ");
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    list_ToolDescription.append(QString("  %1 Werkzeuge EINLAGERN ").arg(in_Size));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    foreach(Tool* tool, toolList_IN->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(tool->get_ToolFL());
    }

    list_ToolDescription.append(" ");
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    list_ToolDescription.append(QString("  %1 Werkzeuge AUSLAGERN ").arg(out_Size));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    foreach(Tool* tool, toolList_OUT->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(QString("%1").arg(tool->get_counter()));
    }
    tableModel = new TableModel(this);

    // Populate model with data:
    tableModel->populateData(list_ToolID,
                             list_ToolDescription,
                             list_ToolGL,
                             list_ToolAL,
                             list_ToolFL);

    // Connect model to table view:
    tableView->setModel(tableModel);

    // Make table header visible and display table:
    //tableView->horizontalHeader()->setVisible(true);
    tableView->show();
}

void MainWindow::showTop100()
{
    list_ToolID.clear();
    list_ToolDescription.clear();
    list_ToolGL.clear();
    list_ToolAL.clear();
    list_ToolFL.clear();
    int magazinSize = toolList_Top100->getSize();

    list_ToolDescription.append(QString("        Top 100:    %1  von 100 belegt").arg(magazinSize));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");
    foreach(Tool* tool, toolList_Top100->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(QString("%1").arg(tool->get_counter()));
    }

    tableModel = new TableModel(this);

    // Populate model with data:
    tableModel->populateData(list_ToolID,
                             list_ToolDescription,
                             list_ToolGL,
                             list_ToolAL,
                             list_ToolFL);

    // Connect model to table view:
    tableView_Top100->setModel(tableModel);

    // Make table header visible and display table:
    //tableView->horizontalHeader()->setVisible(true);
    tableView_Top100->show();
}

void MainWindow::slot_CopyWerkzeugDB()
{
   QFileInfo fileInfo_WerkzeugDB;
   string_WerkzeugDB_orginal = settings->value("WerkzeugDB", "").toString();
   fileInfo_WerkzeugDB = QFileInfo(QDir::homePath() + "/MainGen/WerkzeugDB/WerkzeugDB.db");
   if(fileInfo_WerkzeugDB.exists())
        if(!QFile::remove(fileInfo_WerkzeugDB.absoluteFilePath()))
        {
            slot_Err(Q_FUNC_INFO + QString(" - konnte alte Sicherheitskopie von WerkzeugDB nicht loeschen"));
            return;
        }


   if(!QFile::copy(string_WerkzeugDB_orginal, QDir::homePath() + "/MainGen/WerkzeugDB/WerkzeugDB.db"))
   {
       slot_Err(Q_FUNC_INFO + QString(" - konnte keine Sicherheitskopie von WerkzeugDB erstellen"));
       return;
   }
}

void MainWindow::slot_Erstelle_Top100()
{
    //qDebug() << Q_FUNC_INFO << " start";
    toolList_Top100->clear();
    dbManager->getTop100(toolList_Top100);
    showTop100();
    //qDebug() << Q_FUNC_INFO << " end";
}

void MainWindow::slot_Import_Ruestplan(bool b)
{
    Q_UNUSED(b);

    /*Übergibt dem Parser Ruestplaene/E123456789E01_Sp1.rpl und die ToolListe des Projects zum Speichern*/
    parser->save(QDir::homePath()+ "/MainGen/Ruestplaene/" + string_Projekt + "_" + string_WiederholFertigung + ".rpl", toolList_Project);

    /*Übergibt dem DBManager den Projektnamen und die toolList zum Eintragen*/
    dbManager->insertProject(string_Projekt + "_" + string_WiederholFertigung, toolList_Project);

    QString string_Meldung = QString("| Rüstplan " + string_Projekt + " erfolgreich Importiert |");
    QString string_Balken;
    int i = 0;
    while (i < string_Meldung.size())
    {
        string_Balken = string_Balken + "-";
        i++;
    }

    slot_Log("");
    slot_Log(string_Balken);
    slot_Log(string_Meldung);
    slot_Log(string_Balken);

    tabWidget->setCurrentWidget(widget_Log);
}

void MainWindow::slot_Log(QString str)
{
    textEdit_Log->setTextColor(Qt::black);
    textEdit_Log->append(str);
}

void MainWindow::slot_Err(QString str)
{
        textEdit_Log->setTextColor(Qt::red);
        tabWidget->setCurrentWidget(widget_Log);
        textEdit_Log->append(str);
}

void MainWindow::slot_Print(bool b)
{   Q_UNUSED(b);

    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(slot_PrintPage(QPrinter*)));
    dialog.exec();
}

void MainWindow::slot_PrintPage(QPrinter *printer)
{

    // ------------------ simplest example --------------------------

    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "can't start printer";
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 5 << 3 << 2 << 2 << 20;
    QVector<QString> columnHeaders = QVector<QString>() << "Tool ID" << "GL" << "AL" << "FL" << "Beschreibung";
    if(!tablePrinter.printTable(tableView->model(), columnStretch, columnHeaders)) {
        slot_Err(Q_FUNC_INFO + QString(" - ") + tablePrinter.lastError());
    }
    painter.end();
}

void MainWindow::slot_RecoverDB(bool b)
{
    Q_UNUSED(b);
    dbManager->clearDB();

    QStringList stringList_txtFiles;
    QFileInfoList fileInfoList_txtFiles;
    QDir dir_ruestplanDir;
    //QString string_RuestplanID;

    dir_ruestplanDir.setPath(QDir::homePath() + "/MainGen/Ruestplaene");
    if(!dir_ruestplanDir.exists())
    {
        slot_Err(Q_FUNC_INFO + QString(" - Verzeichnis: ") + QDir::homePath() + QString("/MainGen/Ruestplaene ist nicht vorhanden"));
        return;
    }

    fileInfoList_txtFiles = dir_ruestplanDir.entryInfoList();
    //parser->parse(fileInfoList_txtFiles.first().absoluteFilePath(), toolList_Top100);
    foreach(QFileInfo fileInfo, fileInfoList_txtFiles)
    {
        if(fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            if(fileInfo.suffix() == "rpl")
            {
              stringList_txtFiles.append(fileInfo.absoluteFilePath());
              //qDebug() << fileInfo.absoluteFilePath();
            }
        }
    }

    dialogRecover->recover(stringList_txtFiles);
}

void MainWindow::slot_SchwesterProjekt(bool b)
{
    Q_UNUSED(b);

    QString string_SchwesterProjekt =
            fileDialog->getOpenFileName(this,"Schwester Projekt",QDir::homePath() +"/MainGen/Ruestplaene",tr("Rüstplan(*.rpl)"));

    parser->parse(string_SchwesterProjekt, toolList_AllProjects);
    toolList_AllProjects->sort_ID();
    showTable();
}

void MainWindow::slot_OpenRPL(bool b)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(b);
    QStringList stringList_Split;

    string_Projekt =
            fileDialog->getOpenFileName(this,"Schwester Projekt",QDir::homePath() +"/MainGen/Ruestplaene",tr("Rüstplan(*.rpl)"));
    QFileInfo fileInfo = QFileInfo(string_Projekt);
    stringList_Split = fileInfo.baseName().split("_");
    if(stringList_Split.size() < 4)
    {
        slot_Err("Falscher DateiName: Zeichnungsnummer_Zeichnungsstand_Spannung_Wiederholfertigung");
        slot_Err("E14207809_E02_Sp2_0");
        return;
    }
    string_Projekt = stringList_Split.at(0) + "_" +
                     stringList_Split.at(1) + "_" +
                     stringList_Split.at(2);

    int wf = dbManager->getWiederholFertigung(string_Projekt);
    string_WiederholFertigung = QString("%1").arg(wf);

    qDebug() << "string_Projekt: " << string_Projekt;
    qDebug() << "string_Widerholfertigung: " << string_WiederholFertigung;

    toolList_Project->clear();
    toolList_AllProjects->clear();

    parser->parse(fileInfo.absoluteFilePath(), toolList_Project);
    toolList_Project->sort_ID();

    foreach(Tool* tool, toolList_Project->getList())
        toolList_AllProjects->insert_Tool(tool);

    showTable();
    tabWidget->setCurrentWidget(widget_Ruestplan);
    ui->actionSpeichern->setDisabled(true);
    action_Save->setDisabled(true);
}

void MainWindow::slot_CheckFiles(bool b)
{
    Q_UNUSED(b);
    dialogStart->hide();
    tabWidget->setCurrentWidget(widget_Log);

    load_Programme();
    foreach (QString string_Programm, stringList_Programme)
    {
        parser_Programm->finish(string_ProgrammDir+ "/" + string_Programm);
    }


    //this->close();
}

void MainWindow::slot_ShowSettings(bool b)
{
    Q_UNUSED(b);
    dialogSettings->show();
}

void MainWindow::slot_LicenseFaild()
{
    dialogStart->close();
    slot_Err("Keine gültige Lizenz");
    QTimer::singleShot(5000, this, SLOT(close()));
}

void MainWindow::writeG55()
{
    QFile file(QDir::homePath() + "/MainGen/config/G55.txt");
    QTextStream in(&file);
    QString string_Line;
    QString string_Z = QString("%1").arg(dialogStart->lineEdit_RohteilZ->text().toInt());

    if(!file.exists())
    {
        slot_Err(Q_FUNC_INFO + QString(" - Datei: ") +QDir::homePath() + QString("/MainGen/config/G55.txt ist nicht vorhanden"));
        return;
    }

    if(!file.open(QFile::ReadOnly))
    {
        slot_Err(Q_FUNC_INFO + QString(" - Datei: ") +QDir::homePath() + QString("/MainGen/config/G55.txt konnte nicht geöffnet werden"));
        return;
    }

    while (!in.atEnd())
    {
        string_Line = in.readLine();
        if(string_Line.startsWith("#"))
            continue;

        if(string_Line.contains("$Z$"))
        {
            string_Line.replace("$Z$", string_Z);
        }
        //textEdit_append(string_Line);
        textEdit->append(string_Line);
    }
    file.close();
}

/*void MainWindow::textEdit_append(QString str)
{
    int len = str.length();
    if(len < 75)
    {
        while(len < 75)
        {
            str = str.append(" ");
            len++;
        }
    }
    textEdit->append(str);
}*/

QString MainWindow::formatString(QString str)
{
    int len = str.length();
    if(len < 75)
    {
        while(len < 75)
        {
            str = str.append(" ");
            len++;
        }
    }

    len = str.length();
    while(len > 75 && str.right(1) == " ")
    {
        str = str.left(len - 1);
        len = str.length();
    }
    return str;
}

