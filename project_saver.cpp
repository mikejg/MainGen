#include "project_saver.h"

Project_Saver::Project_Saver(QObject *parent)
    : QObject{parent}
{
    parser_Programm = new Parser_Programm(this);
    connect(parser_Programm,  SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(parser_Programm,  SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));

    parser_PlaceHolder = new Parser_PlaceHolder(this);
    connect(parser_PlaceHolder,  SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(parser_PlaceHolder,  SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));

    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));

    timer = new QTimer(this);
}


QString Project_Saver::formatString(QString str)
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

void Project_Saver::init_Saving()
{
    /*Zähler wird auf Null gesetzt
     * Die Länge des Fortschritsbalken ist die anzahl der Programme
     * Zeige den Dialog an
     * starte slot_nextProgramm*/
    counter = 0;
    dialogProgress->initDialog(stringList_Programme);
    dialogProgress->show();
    timer->singleShot(0, this, SLOT(slot_nextProgramm()));
}

bool Project_Saver::finish_Cleaning()
{
    QStringList stringList_Content;
    mfile->setFileName(string_Destination + "/_Sp1_Reinigen.SPF");
    if(!mfile->read_Content())
        return false;
    stringList_Content = mfile->get_Content();
    stringList_Content = parser_PlaceHolder->set_PlaceHolder_Cleaning(stringList_Content, project);
    mfile->save(stringList_Content);
    return true;
}

bool Project_Saver::finish_BlankControl()
{
    QStringList stringList_Content;
    mfile->setFileName(string_Destination + "/_Sp1_" + project->get_RohteilKontrolle() + ".SPF");
    if(!mfile->read_Content())
        return false;
    stringList_Content = mfile->get_Content();
    stringList_Content = parser_PlaceHolder->set_PlaceHolder_BlankControl(stringList_Content, project);
    mfile->save(stringList_Content);
    return true;
}

bool Project_Saver::finish_Load()
{
    QString string_Loading_Source;
    QString string_Loading_Destination;

    QStringList stringList_Head;
    QStringList stringList_Body;
    QStringList stringList_End;
    QStringList stringList_File;

    string_Destination = settings->get_ProgrammDir() + "/" +
                         string_ProjectName + ".WPD/" +
                         string_ProjectFullName + ".WPD";

    /*Kopiert den Ordner Vorlagen.WPD/WKZ_beladen.WPD nach
     * Programme/E123456789.WPD/E123456789_E0_Sp1.WPD/E123456789_E0_Sp1_In.WPD*/

    string_Loading_Destination = string_Destination + "/" + string_ProjectFullName + "_In.WPD";
    string_Loading_Source      = QDir::homePath() + "/MainGen/Vorlage.WPD/WKZ_beladen.WPD";

    QDir dir_Vorlagen(string_Loading_Source);
    dir_Vorlagen.mkpath(string_Loading_Destination);

    foreach (QString f, dir_Vorlagen.entryList(QDir::Files))
    {
        QFile::copy(QDir::homePath() + "/MainGen/Vorlage.WPD/WKZ_beladen.WPD" + QDir::separator() + f,
                    string_Destination + "/" + string_ProjectFullName + "_In.WPD" + QDir::separator() + f);
    }

    // Lese den Inhalt der Datei WKZ_beladen_Kopf.spf in stringList_Head ein
    // Das gleiche gilt für stringList_Body und stringList_End
    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/WKZ_beladen_Kopf.spf");
    if(!mfile->read_Content())
        return false;
    stringList_Head = mfile->get_Content();

    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/WKZ_beladen_Rumpf.spf");
    if(!mfile->read_Content())
        return false;
    stringList_Body = mfile->get_Content();

    mfile->setFileName(QDir::homePath() + "/MainGen/Vorlagen/WKZ_beladen_End.spf");
    if(!mfile->read_Content())
        return false;
    stringList_End = mfile->get_Content();

    mfile->setFileName(string_Destination + "/" + string_ProjectFullName + "_In.WPD" + QDir::separator() + "_WKZ_beladen.spf");

    /*Gehe durch die stringList_Kopf
     * Schreibe string_Line in das File
     * Wenn du zum Marker 'Werkzeugliste Anfang' kommst gehe durch die Liste
     * der einzulagernden Werkzeuge. */
    int count = 1;
    foreach(QString string_Line, stringList_Head)
    {
        stringList_File.append(QString("N%1 ").arg(count) + string_Line + "\n" );
        count++;
        if(string_Line.contains("Werkzeugliste Anfang"))
        {
            foreach(Tool* tool, project->get_ToolList_IN()->getList())
            {
                stringList_File.append(QString("N%1 ").arg(count) +
                    "; T " + tool->get_Number() +
                    "  " + tool->get_Description() +
                    "\n");
                count++;
            }
        }
    }

    /*Geh durch die Liste der einzulagernden Werkzeuge
     * Geh durch stringList_Rumpf
     * Ersetze '$TNumber$ durch die ToolID und ToolBeschreibung
     * Wenn die ToolID mit 29 beginnt (Einlippenbohrer) dann
     * ersetze 'BRUCH' mit 'BRUCH2' */
    foreach (Tool* tool, project->get_ToolList_IN()->getList())
    {
        foreach (QString string_Body, stringList_Body)
        {
            if(string_Body.contains("$TNumber$"))
            {
                string_Body.replace("$TNumber$", tool->get_Number());
                string_Body = string_Body + " ; " + tool->get_Description();
            }

            if(tool->get_Number().startsWith("29_") && string_Body.contains("call"))
                string_Body.replace("BRUCH", "BRUCH2");

            stringList_File.append(QString("N%1 ").arg(count) + string_Body + "\n");
            count++;
        }
    }

    /*Geh durch stringList_End und schreib jede Zeile in das File*/
    foreach(QString string_Line, stringList_End)
    {
        stringList_File.append(QString("N%1 ").arg(count) + string_Line + "\n" );
        count++;
    }

    mfile->save(stringList_File);

    return true;
}

bool Project_Saver::finish_Touch_Sp1()
{
    QStringList stringList_Content;
    mfile->setFileName(string_Destination + "/_Sp1_Rohteil_antasten.SPF");
    if(!mfile->read_Content())
        return false;
    stringList_Content = mfile->get_Content();
    stringList_Content = parser_PlaceHolder->set_PlaceHolder_Antasten(stringList_Content, project);
    mfile->save(stringList_Content);
    return true;
}

bool Project_Saver::save_Rustplan(QString str, ToolList* toolList)
{

    /*Öffnet das File Ruestplaene/E123456789E01_Sp1.rpl
     * Geht durch die ToolList
     * Erzeugt den String string_Tool aus den Werkzeugdaten
     * 60_120_00 ||  Ø 12 Schruppfraeser GP-175487  AL=45 (originale Form)  ||  135.  ||  45.  || 38
     * Schreibt den String in das File
     */

    QFile file(str);
    QString string_Tool;
    QTextStream textStream(&file);

    if(!file.open(QFile::WriteOnly | QFile::Text))
        return false;


        foreach(Tool* tool, toolList->getList())
        {
            string_Tool = tool->get_Number() + " || " +
                          tool->get_Description() + " || " +
                          tool->get_ToolGL() + " || " +
                          tool->get_ToolAL() + " || " +
                          tool->get_ToolFL();
            //qDebug() << string_Tool;
            if(!string_Tool.startsWith(" "))
                textStream << string_Tool << '\n';
        }
        file.close();

    return true;
}

void Project_Saver::set_Project(Project* p)
{
    project = p;
}

void Project_Saver::slot_nextProgramm()
{
    string_Programm = stringList_Programme.at(counter);
    dialogProgress->setValue(string_Programm, counter);

    parser_Programm->finish(string_ProgrammDir+ "/" + string_Programm);

    QFile::copy(string_ProgrammDir + QDir::separator() + string_Programm,
                string_Destination + QDir::separator() + string_Programm);

    QFile::remove(string_ProgrammDir + QDir::separator() + string_Programm);

    // Counter um eins hochzaehlen und Fortschrittbalken aktualisieren
    counter++;
    dialogProgress->update();

    // Wenn Counterr kleiner als stringList_Programme.size eine halbe Sekunde warten,
    // damit der Fortschrittbalken neu angezeigt wird, dann slot_nextProgramm neu aufrufen
    if(counter < stringList_Programme.size())
        timer->singleShot(500, this, SLOT(slot_nextProgramm()));


    else
    {
        // Wenn die Nummerierung der Programme DEAKTIVIERT ist füge vor dem Hauptprgamm einen
        // Unterstrich ein _E12345678_E1_Sp1.MPF
        if(!project->get_Numbering())
        {
            QFile::copy(string_ProgrammDir + QDir::separator() + string_ProjectFullName +".MPF",
                        string_Destination + QDir::separator() + "_" + string_ProjectFullName +".MPF");
            QFile::remove(string_ProgrammDir + QDir::separator() + string_ProjectFullName + ".MPF");
        }
        // Wenn die Nummerierung der Programme AKTIVIERT ist füge vor dem Hauptprgamm "00_" ein
        // 00_E12345678_E1_Sp1.MPF
        else
        {
            QFile::copy(string_ProgrammDir + QDir::separator() + string_ProjectFullName +".MPF",
                        string_Destination + QDir::separator() + "00_" + string_ProjectFullName +".MPF");
            QFile::remove(string_ProgrammDir + QDir::separator() + string_ProjectFullName + ".MPF");
        }

        // Abschiessende Automatiesrung wie Antasten, Rohteilkontrolle und Reinigung für Spannung1
        if(project->get_ProjectClamping().contains("Sp1"))
        {
            if(!finish_Touch_Sp1())
                return;
            if(!finish_BlankControl())
                return;
            if(!finish_Cleaning())
                return;
        }

        if(!finish_Load())
            return;

        // Wenn es die Erst- bzw Wiederholfertigung ist speicher den Rüstplan und das Projekt in die Datenbank
        if(bool_Repetition)
        {
            if(!save_Rustplan(QDir::homePath()+ "/MainGen/Ruestplaene/"
                                   + project->get_ProjectFullName() + "_"
                                   + QString("%1").arg(project->get_RepetitiveManufacturing())
                                   + ".rpl", project->get_ToolList()))
                return;
            dbManager->insertProject(project->get_ProjectFullName() + "_"
                                     + QString("%1").arg(project->get_RepetitiveManufacturing()),
                                     project->get_ToolList(),
                                     project->get_Data());


        }
        dialogProgress->hide();
        //emit sig_Finished();
    }
}

void Project_Saver::slot_Save_Project(bool b)
{

    bool_Repetition = b;
    stringList_Content_MainProgramm = project->get_Content_MainProgramm();

    string_ProgrammDir     = project->get_ProgrammDir();
    string_ProjectName     = project->get_ProjectName();
    string_ProjectFullName = project->get_ProjectFullName();
    string_ProjectClamping = project->get_ProjectClamping();

    stringList_Programme = project->get_Programms();
    /*Erzeugt im Ordern Programme den Ordner ProjektName.WPD/SpannungX.WPD
     * Programme/E123456789.WPD/Spannung1.WPD
     * Erstellt QDir dir_Vorlagen mit dem Pfad, in dem die Vorlagen für die Spannung sind
     * Erstellt QDir dir_Programme mit dem Pfad 'Programme/E123456789.WPD/Spannung1.WPD'
    */

    /*Bereite den Parser_Programm für die Bruchkontrolle vor*/
    if(!parser_Programm->loadBruch())
        return;

    string_Destination = settings->get_ProgrammDir() + "/" +
                         string_ProjectName + ".WPD/" +
                         string_ProjectFullName + ".WPD";

    string_Source = QDir::homePath() + "/MainGen/Vorlage.WPD/" + string_ProjectClamping + ".WPD";

    QDir dir_Vorlagen(string_Source);
    dir_Vorlagen.mkpath(string_Destination);


    if(!stringList_Programme.isEmpty())
    {
        /*Öffnet das File Programme/00_E123456789.MPF"
        * Schreibt alles aus dem Reiter Hauptprgramm in das File*/
        QString str = settings->get_ProgrammDir() + "/" + string_ProjectFullName +".MPF";

        QFile file(str);
        if(file.open(QFile::WriteOnly))
        {
            marker = keinMarker;
            QTextStream stream(&file);
            foreach(QString string_Line, stringList_Content_MainProgramm)
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

        /* Kopiert den Ordner Vorlagen.WPD/Sp1.WPD nach
         * Programme/E123456789.WPD/E123456789_E1_Sp1.WPD*/
        QStringList stringList_Files = dir_Vorlagen.entryList(QDir::Files);
        QStringList stringList_Temp;

        // Durchsuche stringList_Files nach Einträgen die mit Rohteilkontrolle beginnen
        // und speicher sie in stringList_Temp
        foreach (QString str, stringList_Files)
        {
            if(str.contains("Rohteilkontrolle"))
                stringList_Temp.append(str);
        }

        // geh stringList_Temp durch und lösche jeden Eintrag von stringList_Temp in
        // stringList_Files
        foreach (QString str, stringList_Temp)
        {
            stringList_Files.removeAll(str);
        }

        // Füge Rohteilkontrolle aus dem Project in stringList_Files
        stringList_Files.append("_Sp1_" + project->get_RohteilKontrolle() + ".SPF");

        // Kopiere alle Files aus stringList_Files ins ProjectVerzeichnis
        foreach (QString f, stringList_Files)
        {

            QFile::copy(string_Source + QDir::separator() + f, string_Destination + QDir::separator() + f);
        }
        init_Saving();
    }
    else
    {
        if(!finish_Load())
            return;
    }

    return;
}
