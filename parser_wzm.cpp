#include "parser_wzm.h"

Parser_WZM::Parser_WZM(QObject *parent)
    : QObject{parent}
{

}

void Parser_WZM::parse(QString stringFile, ToolList* toolList)
{
    file_fileToParse.setFileName(stringFile);
    //qDebug() << "Parser_WZM::parse( " << stringFile << ")";

    if(!file_fileToParse.exists())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString(" ist nicht vorhanden"));
        return;
    }

    if(!openFile())
    {
        emit sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString("  konnte nicht geöffnet weren"));
        return;
    }

    readFile();
    splitLine(toolList);

    return;
}

void Parser_WZM::readFile()
{
    //qDebug() << "Parser_WZM::readFile()";
    QTextStream in(&file_fileToParse);
    stringList_Lines.clear();
    while (!in.atEnd())
    {
        stringList_Lines.append(in.readLine());
    }
    file_fileToParse.close();
}

bool Parser_WZM::openFile()
{
    if(file_fileToParse.open(QFile::ReadOnly))
        return true;
    else
        return false;
}

void Parser_WZM::splitLine(ToolList* toolList)
{
    QString string_ToolID;
    Tool* tool = new Tool(this);

    /* Lösche die ToolList
     * Gehe durch alle Zeilen des Rüstplans
     */
    toolList->clear();
    foreach(QString string_Line, stringList_Lines)
    {
        if(string_Line.startsWith("$TC_TP2"))
        {
            while(!string_Line.startsWith("\"") && string_Line.size() > 0)
            {
                string_Line = string_Line.right(string_Line.length()-1);
            }
            string_Line.remove("\"");
            string_ToolID=string_Line;
            continue;
        }

        if(string_Line.startsWith(";$A_TOOLMN"))
        {
            while(!string_Line.startsWith("=") && string_Line.size() > 0)
            {
                string_Line = string_Line.right(string_Line.length()-1);
            }
            string_Line.remove("=");
            if(!string_Line.isEmpty() && string_Line != "0")
            {
                tool->clear();
                tool->set_Number(string_ToolID);
                tool->set_Description(dbManager->getDescription(string_ToolID));
                tool->set_counter(dbManager->getCounter(string_ToolID));
                toolList->insert_Tool(tool);
            }
        }
    }
}
