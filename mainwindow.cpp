#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView_Top100->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_Rustplan->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

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

    //dbManager initialisieren
    dbManager = new DBManager(this);
    connect(dbManager, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(dbManager, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //magazin initialisieren
    magazin = new Magazin(this);
    magazin->set_FilePath(settings->get_MagazinDir() + "/Magazin.INI");
    magazin->set_DBManager(dbManager);
    connect(magazin, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(magazin, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //Project initialisieren
    project = new Project(this);
    project->set_Settings(settings);
    project->set_DBManager(dbManager);
    connect(project, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));
    connect(project, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));

    //DialogStart initialisieren
    dialogStart = new DialogStart(this);
    connect(dialogStart, SIGNAL(allValid()), this, SLOT(slot_dialogStart_Closed()));

    //Parser_Variables initialisieren
    parser_PlaceHolder = new Parser_PlaceHolder(this);
    connect(parser_PlaceHolder, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(parser_PlaceHolder, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

    //mfile initialisieren und Connection herstellen
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Err(QString)), this, SLOT(slot_Err(QString)));
    connect(mfile, SIGNAL(sig_Log(QString)), this, SLOT(slot_Log(QString)));

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

void MainWindow::showTable_Rustplan()
{
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
        if(!toolList_Top100->contains(tool) && !toolList_Table->contains(tool))
            toolList_OUT->insert_Tool(tool);
    }
    toolList_OUT->sort_Counter();
    int_Out_Size = toolList_OUT->getSize();

    //erzeuge erste Zeile mit ProjektNamen und Anzahl der verwendeten Werkzeuge
    list_ToolDescription.append(QString(project->get_ProjectName() + "  %1 Werkzeuge").arg(int_Projects_Size));
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

void MainWindow::showTable_Top100()
{
    /* Vorbereitung zum Anzeigen der Top 100 */
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

void MainWindow::slot_dialogStart_Closed()
{
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
  showTable_Top100();

  dialogStart->show();
}

