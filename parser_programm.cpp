#include "parser_programm.h"
#include <QTextStream>
#include <QDir>
#include <QStringEncoder>
Parser_Programm::Parser_Programm(QObject *parent) : QObject(parent)
{
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
}

void Parser_Programm::insertTool(QString str, ToolList* toolList)
{
    QString string_TNumber;
    QStringList stringList_Parts;

    Tool* tool = new Tool(this);

    //Alle Zeichen am Anfang Löschen bis die Zeile mit einem T anfängt
    while(!str.startsWith("T"))
    {
        str = str.right(str.length()-1);
    }

    //Das T löschen
    str = str.right(str.length()-1);

    //Leerzeichen am Anfang löschen
    while(str.startsWith(" "))
    {
        str = str.right(str.length()-1);
    }


    /* Splitte den QString str bei jedem '||' und
         * schreibe den Teilstring in die QStringList stringList_Part */
    stringList_Parts = str.split("||");


    if(stringList_Parts.size() >= 4)
    {
        tool->clear();
        string_TNumber = stringList_Parts.at(0);
        string_TNumber = string_TNumber.remove(" ");
        tool->set_Number(string_TNumber);
        tool->set_ToolGL(stringList_Parts.at(2));
        tool->set_ToolAL(dbManager->getAusspannLaenge(string_TNumber));
        tool->set_ToolFL(dbManager->getFreistellLaenge(string_TNumber));
        tool->set_HalterName(dbManager->getHolder(string_TNumber));
        tool->set_Description(dbManager->getDescription(string_TNumber));
        tool->set_counter(dbManager->getCounter(string_TNumber));
        toolList->insert_Tool(tool);
    }
}

QString Parser_Programm::parseProjectName(QString stringFile)
{
    mfile->setFileName(stringFile);
    QString returnString;
    QString string_TMP;
    QStringList stringList_Split;

    if(!mfile->read_Content())
    {
        //delete mfile;
        return returnString;
    }

    foreach(QString string_Line, mfile->get_Content())
    {
        if(string_Line.contains(";Projekt:"))
        {
            string_TMP = string_Line;
        }
    }

    if(!string_TMP.isEmpty())
    {
        stringList_Split = string_TMP.split(" || ");
        if (stringList_Split.size() >1)
        {
            returnString = stringList_Split.at(1);
        }
    }
    return returnString;
}

bool Parser_Programm::parseProjectTool(QString stringFile, ToolList* toolList)
{
    mfile->setFileName(stringFile);

    if(!mfile->read_Content())
    {
        return false;
    }

    marker = kein_Marker;

    /* geht durch die QStringList stringList_Lines (Programmzeilen siehe readFile)
     * Wenn in der Zeile 'Werkzeugliste Anfang' vorhanden ist wird der Marker auf
     * werkzeugliste_Anfang gesetzt
     * Wenn in der Zeile 'Werkzeugliste Ende' vorhanden ist wird der Marker auf
     * kein_Marker gesetzt und die Schleife beendet
     * Wenn der Marker gleich werkzeugliste_Anfag und
     * QString string_Line '||' enthält wird die Funktion insertTool aufgerufen */

    foreach(QString string_Line, mfile->get_Content())
    {
        if(string_Line.contains("Werkzeugliste Anfang"))
        {
            marker = werkzeugliste_Anfang;
            continue;
        }

        if(string_Line.contains("Werkzeugliste Ende"))
        {
            marker = kein_Marker;
            break;
        }

        if(marker == werkzeugliste_Anfang && string_Line.contains("||"))
        {
            insertTool(string_Line, toolList);
        }
    }
    return true;
}



