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

void Parser_Programm::compare(QStringList stringList_Orginal, QStringList stringList_Copy)
{
    /*
    qDebug() << Q_FUNC_INFO;
    qDebug() << "   " << file_fileToParse.fileName();
    qDebug() << "   " << fileInfo_Target.absoluteFilePath();
    */

    QString string_Source;
    QString string_Target;
    bool firstLog = false;

    for(int i = 0; i< stringList_Orginal.size(); i++)
    {
        string_Source = stringList_Orginal.at(i);
        string_Target = stringList_Copy.at(i);

        if(string_Target != string_Source)
        {

            if(!firstLog)
            {
                emit sig_Log("------------------------------------------------");
                emit sig_Log(mfile->fileName());
                firstLog = true;
            }
            emit sig_Log("   " + string_Source);
            emit sig_Log("   " + string_Target);
        }
    }

    //qDebug() << Q_FUNC_INFO;
}

void Parser_Programm::finish(QString stringFile)
{
    QString string_ToolID;
    QString str;

    bool_ersterBruch = false;
    bool_Messerkopf = false;
    bool_EinlippenBohrer = false;
    marker = kein_Marker;

    mfile->setFileName(stringFile);
    if(!mfile->read_Content())
        return;

    fileInfo_Source = QFileInfo(*mfile);
    fileInfo_Target = QFileInfo(QDir::homePath() + "/MainGen/SicherheitsKopien/" + fileInfo_Source.baseName() +".SPF");


    if(fileInfo_Target.exists())
    {
        qDebug() << "File: " << fileInfo_Target.absoluteFilePath() << "ist schon da";
        QFile::remove(fileInfo_Target.absoluteFilePath());
    }

    //Sicherheitskopie erstellen
    if(!QFile::copy(stringFile, QDir::homePath() + "/MainGen/SicherheitsKopien/" + fileInfo_Source.baseName() +".SPF"))
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - konnte keine Sicherheitskopie erstellen"));
        return;
    }

    /*Geh durch stringList_Lines.
     * Wenn ein String die Zeichenfolge T=" enthält, filter mir die Nummer raus
     * Beispiel: N34 T="90_063_00"
     * setze den marker auf neuesWerkzeug
     * Wenn zum ersten mal call auftaucht ändere es in ;call
     * Wenn marker gleich neuesWerkzeug und call auftaucht dann
     * geh durch map_Bruch und vergleiche string_ToolID mit dem Key
     * wenn da was passt ersetze call"BRUCH" mit dem Value */
    stringList_Content = mfile->get_Content();

    for(int i = 0; i < stringList_Content.size(); i++)
    {
        str = stringList_Content.at(i);
        if(str.contains("T=\""))
        {
            string_ToolID = str;
            while(!string_ToolID.startsWith("\"") && string_ToolID.length()>0)
            {
                string_ToolID = string_ToolID.right(string_ToolID.length()-1);
                //emit sig_Log(string_ToolID);
            }

            while(!string_ToolID.endsWith("\"") && string_ToolID.length()>0)
            {
                string_ToolID = string_ToolID.left(string_ToolID.length()-1);
            }

            //string_ToolID = string_ToolID.remove("T=");
            string_ToolID = string_ToolID.remove("\"");

            marker = neuesWerkzeug;
        }

        if(!bool_ersterBruch && str.contains("call"))
        {
            if(!str.contains(";"))
            {
                str = str.replace("call", ";call");
                stringList_Content.removeAt(i);
                stringList_Content.insert(i,str);
            }
            bool_ersterBruch = true;
        }


        if(marker == neuesWerkzeug && str.contains("call"))
        {
            QMapIterator<QString, QString> iterator(map_Bruch);
            while (iterator.hasNext())
            {
                iterator.next();
                if(string_ToolID.first(iterator.key().length()) == iterator.key())
                {
                    str = str.replace("call\"BRUCH\"", iterator.value());
                    stringList_Content.removeAt(i);
                    stringList_Content.insert(i,str);
                }
            }
            marker = kein_Marker;
        }
    }

    compare(mfile->get_Content(), stringList_Content);
    mfile->save(stringList_Content);

    return;
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

bool Parser_Programm::loadBruch()
{
    /*Öffne das File "/MainGen/config/Bruch.txt" und lies es
     * in die stringList_Lines ein.
     * Geh durch stringList_Lines und splitte jede Zeile.
     * Füge die ersten zwei Strings in QMap map_Bruch */

    map_Bruch.clear();
    QStringList stringList_Bruch;
    mfile->setFileName(QDir::homePath() + "/MainGen/config/Bruch.txt");
    if(!mfile->read_Content())
        return false;

    foreach(string_Line, mfile->get_Content())
    {
        if(string_Line.contains(" || "))
        {
           stringList_Bruch = string_Line.split(" || ");
           if(stringList_Bruch.size()>1)
           {
               map_Bruch.insert(stringList_Bruch.at(0), stringList_Bruch.at(1));
           }
        }
    }

    return true;
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



