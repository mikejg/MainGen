#include "project.h"
#include <QDir>

Project::Project(QObject *parent)
    : QObject{parent}
{
    parser_Programm = new Parser_Programm(this);
    connect(parser_Programm,  SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(parser_Programm,  SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));

    toolList = new ToolList(this);
    string_ProjectZeroPoint = "G___";
    string_RohteilKontrolle = "Rohteilkontrolle00";
}

void Project::clear()
{

}
void Project::frame_Error(QStringList stringList_Errors)
{
    /* Zieht einen ASCII Rahmen um die Fehlermeldungen */

    int     maxLength = 0;
    QString string_Bar;
    bool    bool_Bar = false;

    /* ermittle den längsten String */
    foreach (QString str, stringList_Errors)
    {
        if(str.length() > maxLength)
            maxLength = str.length();
    }

    /* Erzeuge einen Balken mit der länge der
     * längsten Fehlermeldung
     * und hänge am Schluss noch mal 3 '-' an */
    string_Bar = "";
    while(string_Bar.length() <= maxLength)
    {
        string_Bar = string_Bar + "-";
    }
    string_Bar = string_Bar +"---";

    // gib den Balken als obern Balken aus
    emit sig_Err(string_Bar);

    foreach (QString str, stringList_Errors)
    {
        /* Wenn es die erste Fehlermeldung ist setze dir einen Merker */
        if(str == stringList_Errors.first())
            bool_Bar = true;

        /* Verlängere die Fehlermeldung mit Leerzeichen auf die
         * Länge des Balkens.
         * Hänge am Anfang und am Ende ein '|' an und gib die
         * Fehlermeldung aus */
        while(str.length() < maxLength)
            str = str + " ";
        emit sig_Err("| " + str + " |");

        /* Wenn es sich um die erste Fehlermeldung gib einen Balken aus */
        if(bool_Bar)
        {
            emit sig_Err(string_Bar);
            bool_Bar = false;
        }
    }

    /* gib den Balken als unteren Balken aus */
    emit sig_Err(string_Bar);

    //this->setDisabled(true);
}

bool Project::loadProjectData()
{
    QDir        dir_ProgrammDir;
    QStringList filters;
    QStringList stringList_TMP;
    QStringList stringList_Projektnames;

    bool bool_first = true;
    //Nimm nur SPF-Files
    filters << "*.spf";
    dir_ProgrammDir.setNameFilters(filters);

    /* Erstellt ein Verzeichnis in dem die CNC-Programme sind
     * lädt alle CNC-Prammnamen in die stringList_Prg */
    dir_ProgrammDir.setPath(string_ProgrammDir);
    stringList_Prg.clear();
    stringList_Prg = dir_ProgrammDir.entryList(QDir::Files);

    /* Lösche alle Einträge in der StringList Projektnames */
    stringList_Projektnames.clear();

    /* Geh durch alle gefunden SPF-Dateien und such nach einen Projektnamen
     * Wenn es die erste SPF-Datei ist schreibe den Projektnamen in die Liste
     * ansonsten sieh in der Liste nach ob es den aktuellen Prjektnamen schon gibt
     * Wenn nicht sende eine Fehlermeldung*/
    foreach (QString string_Prg, stringList_Prg)
    {
        string_ProjectName = parser_Programm->parseProjectName(dir_ProgrammDir.path() + "/" + string_Prg);
        if(bool_first)
        {
            stringList_Projektnames.append(string_ProjectName);
            bool_first = false;
            continue;
        }

        if(!stringList_Projektnames.contains(string_ProjectName))
        {
            emit sig_Err("");
            emit sig_Err("-------------------------------------------------------");
            emit sig_Err("|   SPF-Dateien unterschiedlicher Projekte gefunden   |");
            emit sig_Err("-------------------------------------------------------");
            emit sig_Err("| - Lösche SPF-Dateien aus dem Programm Ordner        |");
            emit sig_Err("| - Führen Sie erneut den Postprozessor aus           |");
            emit sig_Err("| - Starten Sie diese Applikation neu                 |");
            emit sig_Err("-------------------------------------------------------");
            return false;
        }
    }

    /* Wenn die stringList_Prg Einträge hat, also wenn CNC-Programme
     * vorhanden sind, lade mir aus dem ersten CNC-Programm den Projektnamen */
    if(!stringList_Prg.isEmpty())
    {

        //string_Projektname = "nichts";
        /* Zieh den ProjektNamen und ProjektStand aus dem CNC-Programm */
        string_ProjectName = parser_Programm->parseProjectName(dir_ProgrammDir.path() + "/" + stringList_Prg.first());

        /* Ersetze alle Leerzeichen durch einen Unterstrich */
        string_ProjectName = string_ProjectName.replace(" ", "_");

        /* Splitte den String nach "_"
         * Erster Teil ist der ProjektName
         * Zweiter Teil ist der ProjektStand */
        if(!string_ProjectName.isEmpty())
        {
            stringList_TMP = string_ProjectName.split("_");
            if(stringList_TMP.size()>0)
            {
                string_ProjectName = stringList_TMP.at(0);
                //emit sig_Log("ProjectName:     " + string_ProjectName);
            }
            if(stringList_TMP.size()>1)
            {
                string_ProjectStatus = stringList_TMP.at(1);
                //emit sig_Log("ProjectStatus:   " + string_ProjectStatus);
            }
            if(stringList_TMP.size()>2)
            {
                string_ProjectClamping = stringList_TMP.at(2);
                //emit sig_Log("ProjectClamping: " + string_ProjectClamping);
            }

            string_ProjectFullName = string_ProjectName + "_" +
                                     string_ProjectStatus + "_" +
                                     string_ProjectClamping;
        }
    }

    if(!sort_Programms())
        return false;
    return true;
}

void Project::logProjectData()
{
    emit sig_Log("ProjectName:     " + string_ProjectName);
    emit sig_Log("ProjectStatus:   " + string_ProjectStatus);
    emit sig_Log("ProjectClamping: " + string_ProjectClamping);
    emit sig_Log("Hauptprogramm :  " + string_MainProgramm);
    emit sig_Log("Material         " + string_Material);
    emit sig_Log("Nullpunkt:       " + string_ProjectZeroPoint);
    emit sig_Log("Rohteil:         " + string_RTx + " x " + string_RTy + " x " + string_RTz);
    emit sig_Log("Bauteil:         " + string_BTx + " x " + string_BTy + " x " + string_BTz);
    emit sig_Log("AntastPunkt Z    " + string_ZRT);
    emit sig_Log("Wiederholfertig  " + QString("%1").arg(int_RepetitiveManufacturing));

    emit sig_Log("\nAufmasse vom Bauteil");
    emit sig_Log("XPlus_Max        " + string_XPlus_Max);
    emit sig_Log("XPlus_Min        " + string_XPlus_Min);
    emit sig_Log("YPlus_Max        " + string_YPlus_Max);
    emit sig_Log("YPlus_Min        " + string_YPlus_Min);
    emit sig_Log("ZPlus_Max        " + string_ZPlus_Max);
    emit sig_Log("ZPlus_Min        " + string_ZPlus_Min);

    emit sig_Log("XMinus_Max       " + string_XMinus_Max);
    emit sig_Log("XMinus_Min       " + string_XMinus_Min);
    emit sig_Log("YMinus_Max       " + string_YMinus_Max);
    emit sig_Log("YMinus_Min       " + string_YMinus_Min);

    // Liste alle Programme aus stringList_Prg auf
    emit sig_Log("Programme:");
    foreach(QString str, stringList_Prg)
    {
        emit sig_Log(str);
    }

    // Liste alle Tools auf
    emit sig_Log("\nTools:");
    foreach(Tool* tool, toolList->getList())
    {
        emit sig_Log(tool->get_Number() + " " + tool->get_Description());
    }
}

bool Project::loadProjectToolList()
{
        toolList->clear();
        foreach (QString str, stringList_Prg)
        {
            if(!parser_Programm->parseProjectTool(string_ProgrammDir + "/"+ str, toolList))
            return false;
        }
        toolList->sort_ID();
        return true;
}

void Project::set_Content_MainProgramm(QTextEdit* textEdit)
{
    stringList_Content_MainProgramm = textEdit->toPlainText().split("\n");
}

void Project::set_DBManager(DBManager* dbm)
{
    dbManager = dbm;
    parser_Programm->setDBManager(dbm);
}
void Project::set_ProjectFullName()
{
        string_ProjectFullName = string_ProjectName + "_" +
                                 string_ProjectStatus + "_" +
                                 string_ProjectClamping;
}

void Project::set_RepetitiveManufacturing()
{
    int_RepetitiveManufacturing = dbManager->getWiederholFertigung(string_ProjectFullName);
}

void Project::set_Settings(Settings* s)
{
    settings = s;
    string_ProgrammDir = settings->get_ProgrammDir();
    bool_AufmassMax    = settings->get_AufmassMax();
    bool_Numbering     = settings->get_Numbering();
}

bool Project::sort_Programms()
{
    QDir        dir_ProgrammDir;
    QStringList filters;
    QString     string_shortName;
    int         int_I;  Q_UNUSED(int_I);
    bool        bool_OK;

    dir_ProgrammDir.setPath(string_ProgrammDir);
    filters << "*.spf";
    dir_ProgrammDir.setNameFilters(filters);

    /* Lösche alle einträge in stringList_Prg
     * lade alle ProgrammNamen in stringList_Programme*/
    stringList_Prg.clear();
    stringList_Prg = dir_ProgrammDir.entryList(QDir::Files);

    /* Erzeuge eine temporäre StringList tmp
     * geh durch stringList_Programme
     * wenn der Eintrag mit 1_ - 9_ stratet setze eine 0 davor
     * Überprüfe die länge des Progammnamens abhängig ob
     * eine Nummerierung vorgesehen ist oder nicht
     * schreibe den Eintrag in tmp*/
    QStringList tmp;
    foreach(QString str, stringList_Prg)
    {
        if (str.startsWith("1_") || str.startsWith("2_") || str.startsWith("3_") ||
            str.startsWith("4_") || str.startsWith("5_") || str.startsWith("6_") ||
            str.startsWith("7_") || str.startsWith("8_") || str.startsWith("9_") )
        {
            dir_ProgrammDir.rename(str, "0"+str);
            str = "0" + str;
        }

        if(!bool_Numbering && str.length() > 31)
        {
            if(str.startsWith("0"))
            {
                str = str.right(str.length()-1);
                dir_ProgrammDir.rename("0"+ str, str);
            }
            QStringList stringList_Errors;
            stringList_Errors.append("Datei: " + str + " zu lang");
            stringList_Errors.append(" - Loeschen Sie die Datei");
            stringList_Errors.append(" - Kürzen Sie den Komponentenjob");
            stringList_Errors.append(" - Spielen sie die Datei neu aus");
            stringList_Errors.append(" - Starten Sie die Applikation neu");
            frame_Error(stringList_Errors);
            return false;
        }

        if(bool_Numbering && str.length() > 28)
        {
            qDebug() << str << ": " << str.length();
            if(str.startsWith("0"))
            {
                str = str.right(str.length()-1);
                dir_ProgrammDir.rename("0"+ str, str);
            }
            QStringList stringList_Errors;
            stringList_Errors.append("Datei: " + str + " zu lang");
            stringList_Errors.append(" - Loeschen Sie die Datei");
            stringList_Errors.append(" - Kürzen Sie den Komponentenjob");
            stringList_Errors.append(" - Spielen sie die Datei neu aus");
            stringList_Errors.append(" - Starten Sie die Applikation neu");
            frame_Error(stringList_Errors);
            return false;
            }

            tmp.append(str);
        }

    /* Sortiere tmp
     * Lösche alle Einträge in stringList_Prg
     * geht durch tmp
     * Schreibe alle Einträge von tmp in stringList_Prg*/
    tmp.sort();
    stringList_Prg.clear();

    foreach(QString str, tmp)
    {
        /* Wenn keine Nummerierung der Programme vorgesehen ist
         * lösche die Nummerierung vor dem ProgrammNamen */
        if(!bool_Numbering)
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
            dir_ProgrammDir.rename(str, string_shortName);

            /* trage string_shortName in stringList_Progamme ein */
            stringList_Prg.append(string_shortName);
        }
    else
        {
            /* trage str in d stringList_Programme ein */
            stringList_Prg.append(str);
        }
    }

    return true;
}
