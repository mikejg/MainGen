#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //c_Algin = new C_Algin(ui->tab_C_Algin);

    QToolBar *toolbar = this->addToolBar("main toolbar");
    action_Open = toolbar->addAction(QIcon(":/Icons/File_Open.png"),"Rüstplan Öffnen");
    action_AddFile = toolbar->addAction(QIcon(":/Icons/AddFolder.png"),"Rüstplan hinzufügen");
    action_Export = toolbar->addAction(QIcon(":/Icons/Export.png"),"Packet Exportieren");
    action_Print = toolbar->addAction(QIcon(":/Icons/Print.png"),"Rüstplan Drucken");
    action_FinishFile = toolbar->addAction(QIcon(":/Icons/Inspection.png"),"Finish einzelne Files");
    action_ShowSettings = toolbar->addAction(QIcon(":/Icons/settings.png"),"Einstellungen");
    action_RestoreDatabase = toolbar->addAction(QIcon(":/Icons/database.png"),"Datenbank wiederherstellen");

    //ui->tableView_Top100->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_Rustplan->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //Tab C Ausrichten einrichten


    toolList_IN          = new ToolList(this);
    toolList_OUT         = new ToolList(this);
    toolList_Table       = new ToolList(this);
    toolList_AllProjects = new ToolList(this);

    paletteInValid = new QPalette();
    paletteInValid->setColor(QPalette::Text,Qt::red);

    paletteValid = new QPalette();
    *paletteValid = palette();

    //Alles was man für die Einstellungen braucht
    settings = new Settings(this);
    dialogSettings = new DialogSettings(this);
    dialogSettings->set_Settings(settings);
    connect(dialogSettings, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(dialogSettings, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(dialogSettings, SIGNAL(settingsOK()),     this, SLOT(slot_startApplication()));

    //Lizenz initialisieren
    license = new License(this);
    license->setSettings(settings);
    connect(license,SIGNAL(sig_LicenseFailed()), this, SLOT(slot_LicenseFaild()));

    //DialogProgress initialisieren
    dialogProgress = new DialogProgress(this);

    //dbManager initialisieren
    dbManager = new DBManager(this);
    dbManager->set_DialogProgress(dialogProgress);
    connect(dbManager, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(dbManager, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //magazin initialisieren
    magazin = ui->tab_Magazin;
    magazin->set_FilePath(settings->get_MagazinDir() + "/Magazin.INI");
    magazin->set_DBManager(dbManager);
    connect(magazin, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(magazin, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //Project initialisieren
    project = new Project(this);
    project->set_Settings(settings);
    project->set_DBManager(dbManager);
    project->set_ToolList_IN(toolList_IN);
    connect(project, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(project, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));

    //DialogStart initialisieren
    dialogStart = new DialogStart(this);
    connect(dialogStart, SIGNAL(allValid()), this, SLOT(slot_dialogStart_Closed()));

    //DialogRepetition initialisieren
    dialogRepetition = new DialogRepetition(this);
    connect(dialogRepetition, SIGNAL(accepted()), this, SLOT(slot_RepetitionAccepted()));

    //Parser_Variables initialisieren
    parser_PlaceHolder = new Parser_PlaceHolder(this);
    connect(parser_PlaceHolder, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(parser_PlaceHolder, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //Parser_Programm initialisieren
    parser_Programm = new Parser_Programm(this);
    connect(parser_Programm, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(parser_Programm, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //Project_Saver initialisieren
    project_Saver = new Project_Saver(this);
    project_Saver->set_Project(project);
    project_Saver->set_Settings(settings);
    project_Saver->set_DialogProgress(dialogProgress);
    project_Saver->set_DBManager(dbManager);
    connect(project_Saver, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(project_Saver, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //Project_Loader initialisieren
    project_Loader = new Project_Loader(this);
    project_Loader->set_DBManager(dbManager);
    connect(project_Loader, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(project_Loader, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //mfile initialisieren und Connection herstellen
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(mfile, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //connection Menu
    connect(ui->action_Export, SIGNAL(triggered(bool)), this, SLOT(slot_Export(bool)));
    connect(ui->action_FinishFile, SIGNAL(triggered(bool)), this, SLOT(slot_FinishFile(bool)));
    connect(ui->action_Open_2, SIGNAL(triggered(bool)), this, SLOT(slot_Open(bool)));
    connect(ui->action_Print, SIGNAL(triggered(bool)), this, SLOT(slot_Print(bool)));
    connect(ui->action_Settings, SIGNAL(triggered(bool)), this, SLOT(slot_ShowSettings(bool)));

    //connection ToolBar
    connect(action_AddFile, SIGNAL(triggered(bool)), this, SLOT(slot_AddFile(bool)));
    connect(action_Open, SIGNAL(triggered(bool)), this, SLOT(slot_Open(bool)));
    connect(action_Print, SIGNAL(triggered(bool)), this, SLOT(slot_Print(bool)));
    connect(action_Export, SIGNAL(triggered(bool)), this, SLOT(slot_Export(bool)));
    connect(action_FinishFile, SIGNAL(triggered(bool)), this, SLOT(slot_FinishFile(bool)));
    connect(action_ShowSettings, SIGNAL(triggered(bool)), this, SLOT(slot_ShowSettings(bool)));
    connect(action_RestoreDatabase, SIGNAL(triggered(bool)), this, SLOT(slot_RestoreDatabase(bool)));

    //connect C_Algin
    connect(ui->tab_C_Algin, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(ui->tab_C_Algin, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    QTimer::singleShot(500,this,SLOT(slot_startApplication()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::copyWerkzeugDB()
{
    QFileInfo fileInfo_WerkzeugDB;
    QString string_WerkzeugDB = settings->get_WerkzeugDB();

    /* Wenn das File WerkzeugDB.db schon existiert wird es erstmal gelöscht
     * Wenn das fehlschlägt gib eine Fehlermeldung aus und verlasse die
     * Funktion mit false*/
    fileInfo_WerkzeugDB = QFileInfo(QDir::homePath() + "/MainGen/WerkzeugDB/WerkzeugDB.db");
    if(fileInfo_WerkzeugDB.exists())
        if(!QFile::remove(fileInfo_WerkzeugDB.absoluteFilePath()))
        {
            slot_Err(Q_FUNC_INFO + QString(" - konnte alte Sicherheitskopie von WerkzeugDB nicht loeschen"));
            return false;
        }

    /* Kopiert die orginal Werkzeugdatenbank nach /MainGen/WerkzeugDB/WerkzeugDB.db
     * Wenn das fehlschlägt gib eine Fehlermeldung aus und verlasse die
     * Funktion mit false*/
    if(!QFile::copy(string_WerkzeugDB, QDir::homePath() + "/MainGen/WerkzeugDB/WerkzeugDB.db"))
    {
        slot_Err(Q_FUNC_INFO + QString(" - konnte keine Sicherheitskopie von WerkzeugDB erstellen"));
        return false;
    }
    return true;
}



void MainWindow::FileNameMax(QStringList stringList_Errors)
{
    int maxLength = 0;
    QString string_Balken;
    bool bool_Balken = false;
    foreach (QString str, stringList_Errors)
    {
        if(str.length() > maxLength)
            maxLength = str.length();
    }
    string_Balken = "";
    while(string_Balken.length() <= maxLength)
    {
        string_Balken = string_Balken + "-";
    }
    string_Balken = string_Balken +"---";

    slot_Err(string_Balken);
    foreach (QString str, stringList_Errors)
    {
        if(str == stringList_Errors.first())
            bool_Balken = true;
        while(str.length() < maxLength)
            str = str + " ";
        slot_Err("| " + str + " |");
        if(bool_Balken)
        {
            slot_Err(string_Balken);
            bool_Balken = false;
        }
    }
    slot_Err(string_Balken);
    this->setDisabled(true);
}

bool MainWindow::load_Programme()
{
    /* Erzeuge dir und setze den Pfad auf string_ProgrammDir
     * setz den Filter auf SPF-Dateien */
    QDir dir;
    QStringList filters;
    dir.setPath(settings->get_ProgrammDir());
    filters << "*.spf";
    dir.setNameFilters(filters);

    /* Variable die ich später noch brauche */
    QString string_shortName;
    bool bool_OK;
    int int_I;  Q_UNUSED(int_I);

    /* Lösche alle Programme aus stringList_Programme */
    stringList_Programme.clear();

    stringList_Programme = dir.entryList(QDir::Files);

    /*Erzeuge eine temporäre StringList tmp
     * geh durch stringList_Programme
     * wenn der Eintrag mit 1_ - 9_ stratet setze eine 0 davor
     * Überprüfe die länge des Progammnamens abhängig ob
     *  eine Nummerierung vorgesehen ist oder nicht
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

        if(!settings->get_Numbering() && str.length() > 31)
        {
            if(str.startsWith("0"))
            {
                str = str.right(str.length()-1);
                dir.rename("0"+ str, str);
            }
            QStringList stringList_Errors;
            stringList_Errors.append("Datei: " + str + " zu lang");
            stringList_Errors.append(" - Loeschen Sie die Datei");
            stringList_Errors.append(" - Kürzen Sie den Komponentenjob");
                stringList_Errors.append(" - Spielen sie die Datei neu aus");
            stringList_Errors.append(" - Starten Sie die Applikation neu");
            FileNameMax(stringList_Errors);
            return false;
        }

        if(settings->get_Numbering() && str.length() > 28)
        {
            qDebug() << str << ": " << str.length();
            if(str.startsWith("0"))
            {
                str = str.right(str.length()-1);
                dir.rename("0"+ str, str);
            }
            QStringList stringList_Errors;
            stringList_Errors.append("Datei: " + str + " zu lang");
            stringList_Errors.append(" - Loeschen Sie die Datei");
            stringList_Errors.append(" - Kürzen Sie den Komponentenjob");
                stringList_Errors.append(" - Spielen sie die Datei neu aus");
            stringList_Errors.append(" - Starten Sie die Applikation neu");
            FileNameMax(stringList_Errors);
            return false;
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
        /* Wenn keine Nummerierung der Programme vorgesehen ist */
        if(!settings->get_Numbering())
        {
        /* Kopiere str nach string_shortName */
        string_shortName = str;

        /* Überprüfe ob das erste Zeichen von string_shortName eine
             * Zahl ist. Das Ergebniss wird in bool_OK gespeichert */
        int_I = string_shortName.left(1).toInt(&bool_OK, 10);

        while(bool_OK)
        {
            /* Lösche das erste Zeichen von string_shortName */
                string_shortName = string_shortName.right(string_shortName.length()-1);

                /* Überprüfe ob das erste Zeichen von string_shortName eine Zahl ist */
            int_I = string_shortName.left(1).toInt(&bool_OK, 10);

            /* Wenn das erste Zeichen ein '_' ist wird bool_OK auf true gesetzt */
            if(string_shortName.startsWith("_"))
                bool_OK = true;
        }

        /* Benenne das SPF-File um */
        dir.rename(str, string_shortName);

        /* trage string_shortName in stringList_Progamme ein */
        stringList_Programme.append(string_shortName);
        }
        else
        {
            /* trage str in d stringList_Programme ein */
            stringList_Programme.append(str);
        }
    }
    return true;
}

void MainWindow::showTable_Rustplan()
{
    qDebug() << Q_FUNC_INFO;
    /* Vorbereitung für den Rüstplan */
    toolList_IN->clear();
    toolList_OUT->clear();
    toolList_Table->clear();
    list_ToolID.clear();
    list_ToolDescription.clear();
    list_ToolGL.clear();
    list_ToolAL.clear();
    list_ToolFL.clear();

    int int_Projects_Size;
    int int_In_Size;
    int int_Out_Size;
    int int_Free_Size;

    //Kopiere die ToolList aus dem Projekt in toolList_Table
    //sortiere toolList_Table nach der Tool_ID
    foreach(Tool* tool, project->get_ToolList()->getList())
        toolList_Table->insert_Tool(tool);
    toolList_Table->sort_ID();

    int_Projects_Size = project->get_ToolList()->getSize();

    foreach(Tool* tool, toolList_Table->getList())
    {
        if(!magazin->contains(tool))
            toolList_IN->insert_Tool(tool);
    }
    int_In_Size = toolList_IN->getSize();

    //Vergleiche Werkzeugmagazin mit Top100 und Projekt
    foreach(Tool* tool, magazin->get_ToolList()->getList())
    {
        if(!toolList_Top100->contains(tool) &&
            !toolList_Table->contains(tool) &&
            tool->get_State() == Tool::In)
            toolList_OUT->insert_Tool(tool);
    }
    toolList_OUT->sort_Counter();
    int_Out_Size = toolList_OUT->getSize();

    //erzeuge erste Zeile mit ProjektNamen und Anzahl der verwendeten Werkzeuge
    list_ToolDescription.append(QString(project->get_ProjectFullName() + "  %1 Werkzeuge").arg(int_Projects_Size));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    //Füge die Werkzeuge ein
    foreach(Tool* tool, toolList_Table->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(tool->get_ToolFL());
    }

    //erzeuge eine Leerzeile
    list_ToolDescription.append(" ");
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    //erzeuge eine Zeile: 10 Werkzeuge EINLAGERN - 15 Freie Plätze im Magazin"
    int_Free_Size = settings->get_WerkzeugPlatze() - magazin->get_Size();
    list_ToolDescription.append(QString("  %1 Werkzeuge EINLAGERN  -  %2 Frei Plätze im Magazin").arg(int_In_Size).arg(int_Free_Size));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    //Füge alle Werkzeuge aus der toolList_IN ein
    foreach(Tool* tool, toolList_IN->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(tool->get_ToolFL());
    }

    //erzeuge eine Leerzeile
    list_ToolDescription.append(" ");
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    //erzeuge eine Zeile: 15 Werkzeuge Auslagen
    list_ToolDescription.append(QString("  %1 Werkzeuge AUSLAGERN ").arg(int_Out_Size));
    list_ToolID.append(" ");
    list_ToolGL.append(" ");
    list_ToolAL.append(" ");
    list_ToolFL.append(" ");

    //Füge alle Werkzeuge aus der toolList_Out ein
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

    ui->tableView_Rustplan->setModel(tableModel);
    ui->tableView_Rustplan->show();
}

/*
void MainWindow::showTable_Top100()
{
    // Vorbereitung zum Anzeigen der Top 100
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
    //showTable(toolList_Top100, ui->tableView_Top100);

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
    ui->tableView_Top100->setModel(tableModel);

    // Make table header visible and display table:
    //tableView->horizontalHeader()->setVisible(true);
    ui->tableView_Top100->show();
}
*/

void MainWindow::slot_AddFile(bool b)
{
    Q_UNUSED(b);
    QString string_FileName;
    string_FileName = QFileDialog::getOpenFileName(this,"Schwester Projekt",
                                                   QDir::homePath() +"/MainGen/Ruestplaene",tr("Rüstplan(*.rpl)"));

                      if(!project_Loader->add_Project(string_FileName, project))
                      this->setDisabled(true);

    showTable_Rustplan();
}

void MainWindow::slot_RepetitionAccepted()
{
    /* Abhängig vom radioButton_Repetition wird die Funktion
     * 'save_MPF' mit true oder false aufgerufen */
    if(dialogRepetition->radioButton_Repetition->isChecked())
    {
        project_Saver->slot_Save_Project(true);
    }
    else
    {
        project_Saver->slot_Save_Project(false);
    }
}

void MainWindow::slot_dialogStart_Closed()
{
    qDebug() << Q_FUNC_INFO;
    /* setze mfile den Pfad für das Hauptprogramm
     * lese das Hauptprogramm ein
     * wenn es einen Fehler beim einlesen gibt verlasse die Funktion */
    mfile->setFileName(project->get_MainProgramm());
    if(!mfile->read_Content()) return;

    /* ersetze im Hauptprogramm die Platzhalter */
    QStringList stringList_Content = parser_PlaceHolder->set_PlaceHolder_MainProgramm(mfile->get_Content(),project);

    /* schreibe den Inhalt des Hauptprogramms in den TextEditor */
    foreach(QString str, stringList_Content)
    {
        ui->textEdit->append(str);
    }

    showTable_Rustplan();

}

void MainWindow::slot_Err(QString str)
{
    ui->textEdit_Log->setPalette(*paletteInValid);;
    ui->tabWidget->setCurrentWidget(ui->tab_Log);
    ui->textEdit_Log->append(str);
}

void MainWindow::slot_FinishFile(bool b)
{
    Q_UNUSED(b);
    if(!parser_Programm->loadBruch())
        return;
    if(!load_Programme())
        return;

    ui->tabWidget->setCurrentWidget(ui->tab_Log);
    foreach (QString string_Programm, stringList_Programme)
    {
        parser_Programm->finish(settings->get_ProgrammDir()+ "/" + string_Programm);
    }
}

void MainWindow::slot_Log(QString str)
{
    ui->textEdit_Log->setPalette(*paletteValid);
    ui->textEdit_Log->append(str);
}

void MainWindow::slot_LicenseFaild()
{
    //dialogStart->close();
    slot_Err("Keine gültige Lizenz");
        QTimer::singleShot(5000, this, SLOT(close()));
}

void MainWindow::slot_Open(bool b)
{
    Q_UNUSED(b);
    QString string_FileName;
    string_FileName = QFileDialog::getOpenFileName(this,"Schwester Projekt",
                                                   QDir::homePath() +"/MainGen/Ruestplaene",tr("Rüstplan(*.rpl)"));

    if(!project_Loader->load_Project(string_FileName, project))
        this->setDisabled(true);

    showTable_Rustplan();

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
        if(!tablePrinter.printTable(ui->tableView_Rustplan->model(), columnStretch, columnHeaders)) {
        slot_Err(Q_FUNC_INFO + QString(" - ") + tablePrinter.lastError());
        }
        painter.end();
}

void MainWindow::slot_Export(bool b)
{
        Q_UNUSED(b);

        project->set_Content_MainProgramm(ui->textEdit);
        /* Wenn es keine Wiederholfertigung ist schreibe das Projekt in die Datenbank */

        if(project->get_RepetitiveManufacturing() == 0)
        {
            project_Saver->slot_Save_Project(true);
            return;
        }

        // zeige den Auswahldialog Wiederholfertigung
        dialogRepetition->show();
        return;

}

void MainWindow::slot_RestoreDatabase(bool b)
{
        Q_UNUSED(b);
        dbManager->restore();
}
void MainWindow::slot_ShowSettings(bool b)
{
    Q_UNUSED(b);
    dialogSettings->show();
}

void MainWindow::slot_startApplication()
{
  /* Überprüfe die Settings
   * Wenn das fehlschlägt verlasse die Funktion */
  if(!dialogSettings->checkSettings())
  {
    dialogSettings->show();
    return;
  }

  /* Lade alle notwendigen Daten für das Projekt
   * wenn das fehlschlägt brich die Funktion ab */
  if(!project->loadProjectData())
  {
    return;
  }

  /* Lade alle Werkzeuge für das Projekt
   * wenn das fehlschlägt brich die Funktion ab */
  if(!project->loadProjectToolList())
  {
    return;
  }

  dialogStart->setProject(project);

  /* Lade die Nullpunkte in den DialogStart
   * Wenn das fehlschlägt brich die Funktion ab */
  if(!dialogStart->load_ZeroPoint())
    return;

  /* Lade die MaterialListe in den DialogStart
   * Wenn das fehlschlägt brich die Funktion ab */
  if(!dialogStart->load_Material())
    return;

  /* Kopiere die orginal WerkzeugDatenbank
   * Wenn das fehlschlägt brich die Funktion ab */
  if(!copyWerkzeugDB())
    return;

  /* Erstelle das Magazin
   * Wenn das fehlschlägt brich die Funktion ab */
  if(!magazin->create_ToolList())
    return;

  /* erstelle eine neue ToolList für Top100
   * hol aus der DatenBank die Top 100*/
  toolList_Top100 = new ToolList(this);
  dbManager->getTop100(toolList_Top100);
  //showTable_Top100();

  dialogStart->show();
}

