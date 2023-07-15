#include "parser_placeholder.h"
#include <QDir>

Parser_PlaceHolder::Parser_PlaceHolder(QObject *parent)
    : QObject{parent}
{
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
}

QStringList Parser_PlaceHolder::set_PlaceHolder_Antasten(QStringList stringList_Content, Project* project)
{
    QStringList stringList_ReturnList;

    foreach(QString string_Line, stringList_Content)
    {
        if(string_Line.startsWith("#"))
            continue;

        string_Line = string_Line.replace("$RX$", project->get_RohteilX());
        string_Line = string_Line.replace("$RY$", project->get_RohteilY());
        string_Line = string_Line.replace("$ZRT$", project->get_ZRohteil());
        stringList_ReturnList.append(string_Line);
    }

    return stringList_ReturnList;
}

QStringList Parser_PlaceHolder::set_PlaceHolder_Cleaning(QStringList stringList_Content, Project* project)
{
    QStringList stringList_ReturnList;

    foreach(QString string_Line, stringList_Content)
    {
        if(string_Line.startsWith("#"))
            continue;

        string_Line = string_Line.replace("$RZ$", project->get_RohteilZ());
        stringList_ReturnList.append(string_Line);
    }

    return stringList_ReturnList;
}

QStringList Parser_PlaceHolder::set_PlaceHolder_BlankControl(QStringList stringList_Content, Project* project)
{
    QStringList stringList_ReturnList;

    foreach(QString string_Line, stringList_Content)
    {
        if(string_Line.startsWith("#"))
            continue;

        string_Line = string_Line.replace("$ZRT$", project->get_ZRohteil());
        //Rohteil
        string_Line = string_Line.replace("$RX$", project->get_RohteilX());
        string_Line = string_Line.replace("$RY$", project->get_RohteilY());
        string_Line = string_Line.replace("$RZ$", project->get_RohteilZ());

        //Fertigteil
        string_Line = string_Line.replace("$BX$", project->get_BauteilX());
        string_Line = string_Line.replace("$BY$", project->get_BauteilY());
        string_Line = string_Line.replace("$BZ$", project->get_BauteilZ());

        string_Line = string_Line.replace("$Aufmass_Xplus_Max$",  project->get_XPlus_Max());
        string_Line = string_Line.replace("$Aufmass_Xminus_Max$", project->get_XMinus_Max());
        string_Line = string_Line.replace("$Aufmass_Yplus_Max$",  project->get_YPlus_Max());
        string_Line = string_Line.replace("$Aufmass_Yminus_Max$", project->get_YMinus_Max());
        string_Line = string_Line.replace("$Aufmass_Zplus_Max$",  project->get_ZPlus_Max());

        string_Line= string_Line.replace("$Aufmass_Xplus_Min$" , project->get_XPlus_Min());
        string_Line= string_Line.replace("$Aufmass_Xminus_Min$", project->get_XMinus_Min());
        string_Line= string_Line.replace("$Aufmass_Yplus_Min$" , project->get_YPlus_Min());
        string_Line= string_Line.replace("$Aufmass_Yminus_Min$", project->get_YMinus_Min());
        string_Line= string_Line.replace("$Aufmass_Zplus_Min$",  project->get_ZPlus_Min());
        stringList_ReturnList.append(string_Line);
    }

    return stringList_ReturnList;
}

QStringList Parser_PlaceHolder::set_PlaceHolder_MainProgramm(QStringList stringList_Content, Project* project)
{
    // stringList_Content in stringList_tmp kopieren
    QStringList stringList_tmp = stringList_Content;

    // Inhalt von stringList_Content löschen
    stringList_Content.clear();

    foreach(QString string_Line, stringList_tmp)
    {
        //Wenn die Zeile eine # enthält wird die Zeile ignoriert
        if(string_Line.startsWith("#"))
            continue;

        /* Ersetze die Platzhalter */
        string_Line = string_Line.replace("$ZNr$",  project->get_ProjectName());
        string_Line = string_Line.replace("$ZStd$", project->get_ProjectStatus());
        string_Line = string_Line.replace("$RX$",   project->get_RohteilX());
        string_Line = string_Line.replace("$RY$",   project->get_RohteilY());
        string_Line = string_Line.replace("$RZ$",   project->get_RohteilZ());
        string_Line = string_Line.replace("$G5x$",  project->get_ProjectZeroPoint());
        string_Line = string_Line.replace("$ZRT$",  project->get_ZRohteil());
        string_Line = string_Line.replace("$Ma$",   project->get_Material());

        /* Wenn die Zeile $G55$ enthält wird der Inhalt der Datei
         * "/MainGen/config/G55.txt" ausgegeben.
         * Falls das fehlschlägt wird die eine leere StringList
         * zurückgegeben und eine Fehlermeldung angezeigt */
        if(string_Line.contains("$G55$"))
        {
            if(writeG55(&stringList_Content, project))
                continue;
            else
            {
                stringList_Content.clear();
                return stringList_Content;
            }
        }

        /* füge string_Line in stringList_Content ein */
        stringList_Content.append(string_Line);

        /* Wenn die Zeile ';Programme' enthält wird eine Liste der
         * Unterprogramme ausgegeben */
        if(string_Line.contains(";Programme"))
        {
            foreach(QString str, project->get_Programms())
            {
                string_Line = "call \"" + str.left(str.length()-4) + "\"\n";
                stringList_Content.append(string_Line);
            }
        }
    }

    return stringList_Content;
}

bool Parser_PlaceHolder::writeG55(QStringList* stringList_Content, Project* p)
{
    QStringList stringList_ContentG55;

    /* Versuche das File "/MainGen/config/G55.txt" zu öffnen
     * und den Inhalt einzulesen.
     * Wenn das fehl schlägt gib false zurück
     * Übergib den Inhalt der Datei an stringList_ContentG55 */
    mfile->setFileName(QDir::homePath() + "/MainGen/config/G55.txt");
    if(!mfile->read_Content())
        return false;
    stringList_ContentG55 = mfile->get_Content();


    foreach(QString string_Line, stringList_ContentG55)
    {
        //Wenn die Zeile eine # enthält wird die Zeile ignoriert
        if(string_Line.startsWith("#"))
            continue;

        /* Ersetze den Platzhalter '$Z$' mit der Rohteil Höhe */
        if(string_Line.contains("$Z$"))
        {
            string_Line.replace("$Z$", p->get_RohteilZ());
        }

        /* füge string_Line in stringList_Content ein */
        stringList_Content->append(string_Line);
    }
    return true;
}
