#include "parser_programm.h"
#include <QTextStream>
#include <QDir>
#include <QStringEncoder>
Parser_Programm::Parser_Programm(QObject *parent) : QObject(parent)
{
    marker = kein_Marker;
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
            //string_TDescription = stringList_Parts.at(1);
            //string_TDescription = QString::fromUtf8(stringList_Parts.at(1).toLatin1().data());
            //string_TDescription = QString::fromLocal8Bit(string_TDescription.toLatin1().data());
            //string_TDescription.replace("??", "Ø");
            tool->set_Description(dbManager->getDescription(string_TNumber));
            tool->set_counter(dbManager->getCounter(string_TNumber));
            toolList->insert_Tool(tool);
        }

}

bool Parser_Programm::openFile()
{
    /* Wenn sich das File öffnen lässt gib true zurück ansonsten false */
    if(file_fileToParse.open(QFile::ReadOnly))
        return true;
    else
        return false;
}

void Parser_Programm::parse(QString stringFile, ToolList* toolList)
{
    /* erzeuge aus dem String stringFile ein QFile und QFileInfo */
    file_fileToParse.setFileName(stringFile);
    fileInfo = QFileInfo(file_fileToParse);

    /* Wenn das File nicht existiert schick eine Fehlermeldung raus und verlasse die Funktion */
    if(!file_fileToParse.exists())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString(" ist nicht vorhanden"));
        return;
    }

    /* Wenn sich das File nicht öffnen lässt schick eine Fehlermeldung raus und verlasse die Funktion */
    if(!openFile())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString("  konnte nicht geöffnet weren"));
        return;
    }

    readFile();
    findTool(toolList);

    return;
}

QString Parser_Programm::parseProjectName(QString stringFile)
{
    /* erzeuge aus dem String stringFile ein QFile und QFileInfo */
    file_fileToParse.setFileName(stringFile);
    fileInfo = QFileInfo(file_fileToParse);
    QString returnString;
    QString string_TMP;
    QStringList stringList_Split;
    /* Wenn das File nicht existiert schick eine Fehlermeldung raus und verlasse die Funktion */
    if(!file_fileToParse.exists())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString(" ist nicht vorhanden"));
        return returnString;
    }

    /* Wenn sich das File nicht öffnen lässt schick eine Fehlermeldung raus und verlasse die Funktion */
    if(!openFile())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString("  konnte nicht geöffnet weren"));
        return returnString;
    }

    readFile();
    foreach(QString string_Line, stringList_Lines)
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

void Parser_Programm::finish(QString stringFile)
{
    //qDebug() << Q_FUNC_INFO << " " + stringFile;
    file_fileToParse.setFileName(stringFile);
    fileInfo_Source = QFileInfo(file_fileToParse);
    fileInfo_Target = QFileInfo(QDir::homePath() + "/MainGen/SicherheitsKopien/" + fileInfo_Source.baseName() +".SPF");

    if(!openFile())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString("  konnte nicht geöffnet weren"));
        return;
    }

    if(!file_fileToParse.exists())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString(" ist nicht vorhanden"));
        return;
    }

    if(fileInfo_Target.exists())
    {
        //qDebug() << "File: " << fileInfo_Target.absoluteFilePath() << "ist schon da";
        QFile::remove(fileInfo_Target.absoluteFilePath());
    }

    //Sicherheitskopie erstellen
    if(!QFile::copy(stringFile, QDir::homePath() + "/MainGen/SicherheitsKopien/" + fileInfo_Source.baseName() +".SPF"))
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - konnte keine Sicherheitskopie erstellen"));
        return;
    }

    readFile();
    finish();

    return;
}

void Parser_Programm::readFile()
{
    /* Lese Zeile für Zeile und schreibe jede Zeile in die QStringList stringList_Lines
     * schliesse das File */
    QTextStream in(&file_fileToParse);
    in.setAutoDetectUnicode(false);
    in.setEncoding(QStringConverter::System);

    stringList_Lines.clear();
    while (!in.atEnd())
    {
        stringList_Lines.append(in.readLine());
    }
    file_fileToParse.close();
}

void Parser_Programm::findTool(ToolList* toolList)
{
    marker = kein_Marker;
    bool_ersterBruch = false;

    /* geht durch die QStringList stringList_Lines (Programmzeilen siehe readFile)
     * Wenn in der Zeile 'Werkzeugliste Anfang' vorhanden ist wird der Marker auf
     * werkzeugliste_Anfang gesetzt
     * Wenn in der Zeile 'Werkzeugliste Ende' vorhanden ist wird der Marker auf
     * kein_Marker gesetzt und die Schleife beendet
     * Wenn der Marker gleich werkzeugliste_Anfag und
     * QString string_Line '||' enthält wird die Funktion insertTool aufgerufen */

    foreach(QString string_Line, stringList_Lines)
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


}


void Parser_Programm::finish()
{
    bool_ersterBruch = false;
    bool_Messerkopf = false;
    bool_EinlippenBohrer = false;

    QString str;

    for(int i = 0; i < stringList_Lines.size(); i++)
    {
        str = stringList_Lines.at(i);

        if(str.contains("T=\"90_063_00\""))
        {
            bool_Messerkopf = true;
        }

        if(str.contains("T=\"29_"))
        {
            bool_EinlippenBohrer = true;
        }

        if(!bool_ersterBruch && str.contains("call"))
        {
            if(!str.contains(";"))
            {
                str = str.replace("call", ";call");
                stringList_Lines.removeAt(i);
                stringList_Lines.insert(i,str);
            }
            bool_ersterBruch = true;
         }

        if(bool_Messerkopf && str.contains("call"))
        {
            if(!str.contains(";"))
            {
                str = str.replace("call", ";call");
                stringList_Lines.removeAt(i);
                stringList_Lines.insert(i,str);
            }
            bool_Messerkopf = false;
        }

        if(bool_EinlippenBohrer && str.contains("call"))
        {
            if(!str.contains("BRUCH2"))
            {
                str = str.replace("BRUCH", "BRUCH2");
                stringList_Lines.removeAt(i);
                stringList_Lines.insert(i,str);
            }
            bool_EinlippenBohrer = false;
        }
    }
    saveFile();
    compare();
}

void Parser_Programm::saveFile()
{
    file_fileToParse.remove();
    //if(file_fileToParse.open(QFile::WriteOnly | QFile::Text))
    if(file_fileToParse.open(QFile::WriteOnly))
    {
        QTextStream textStream(&file_fileToParse);
        foreach(QString str, stringList_Lines)
        {
            textStream << str << '\n';
        }
        file_fileToParse.close();
    }
}

void Parser_Programm::compare()
{
    /*
    qDebug() << Q_FUNC_INFO;
    qDebug() << "   " << file_fileToParse.fileName();
    qDebug() << "   " << fileInfo_Target.absoluteFilePath();
    */

    QFile file_Source(fileInfo_Target.absoluteFilePath());
    QStringList stringList_Source;
    QStringList stringList_Target;
    QTextStream textStream_Source(&file_Source);
    QTextStream textStream_Target(&file_fileToParse);
    QString string_Source;
    QString string_Target;
    bool firstLog = false;

    if(!file_fileToParse.open(QFile::ReadOnly))
    {
        sig_Err(Q_FUNC_INFO + QString(" - konnte File: ") + file_fileToParse.fileName() + QString(" nicht öffnen"));
        return;
    }

    stringList_Target.clear();
    while (!textStream_Target.atEnd())
    {
        stringList_Target.append(textStream_Target.readLine());
    }
    file_fileToParse.close();

    if(!file_Source.open(QFile::ReadOnly))
    {
        sig_Err(Q_FUNC_INFO + QString(" - konnte File: ") + file_Source.fileName() + QString(" nicht öffnen"));
        return;
    }

    stringList_Source.clear();
    while (!textStream_Source.atEnd())
    {
        stringList_Source.append(textStream_Source.readLine());
    }
    file_Source.close();

    for(int i = 0; i< stringList_Source.size(); i++)
    {
        string_Source = stringList_Source.at(i);
        string_Target = stringList_Target.at(i);

        if(string_Target != string_Source)
        {

            if(!firstLog)
            {
                textEdit_Log->append("------------------------------------------------");
                textEdit_Log->append(fileInfo_Target.absoluteFilePath());
                firstLog = true;
            }
            textEdit_Log->append("   " + string_Source);
            textEdit_Log->append("   " + string_Target);
        }
    }

    //qDebug() << Q_FUNC_INFO;
}
