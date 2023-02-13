#include "parser.h"
#include <QTextStream>
#include <QFileInfo>

Parser::Parser(QObject *parent)
    : QObject{parent}
{

}

void Parser::parse(QString stringFile, ToolList* toolList)
{
    file_fileToParse.setFileName(stringFile);


    if(!file_fileToParse.exists())
    {
        sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString(" ist nicht vorhanden"));
        return;
    }

    if(!openFile())
    {
        sig_Err(Q_FUNC_INFO + QString(" - Datei: ") + stringFile + QString("  konnte nicht geöffnet weren"));
        return;
    }

    readFile();
    splitLine(toolList);

    return;
}

void Parser::readFile()
{
    //qDebug() << "Parser::readFile()";
    QTextStream in(&file_fileToParse);
    stringList_Lines.clear();
    while (!in.atEnd())
    {
        stringList_Lines.append(in.readLine());
    }
    file_fileToParse.close();
}

void Parser::splitLine(ToolList* toolList)
{
    //QString string_TDescription;
    QString string_TNumber;

    QStringList stringList_Parts;
    Tool* tool = new Tool(this);

    foreach(QString string_Line, stringList_Lines)
    {
        //qDebug() << string_Line;
        stringList_Parts = string_Line.split("||");
        if(stringList_Parts.size() >= 4)
        {
            tool->clear();

            string_TNumber = stringList_Parts.at(0);
            string_TNumber = string_TNumber.remove(" ");
            tool->set_Number(string_TNumber);
            tool->set_ToolGL(stringList_Parts.at(2));
            tool->set_ToolAL(stringList_Parts.at(3));
            tool->set_ToolFL(dbManager->getFreistellLaenge(string_TNumber));
            //string_TDescription = stringList_Parts.at(1);
            tool->set_Description(dbManager->getDescription(string_TNumber));
            tool->set_HalterName(dbManager->getHolder(string_TNumber));
            toolList->insert_Tool(tool);
            //sig_Log(Q_FUNC_INFO + string_TNumber + QString(" | ") + tool->get_HalterName());
        }
    }
}

bool Parser::openFile()
{
    if(file_fileToParse.open(QFile::ReadOnly))
        return true;
    else
        return false;
}

void Parser::save(QString str, ToolList* toolList)
{
    /*Öffnet das File Ruestplaene/E123456789E01_Sp1.rpl
     * Geht durch die ToolList
     * Erzeugt den String string_Tool aus den Werkzeugdaten
     * 60_120_00 ||  Ø 12 Schruppfraeser GP-175487  AL=45 (originale Form)  ||  135.  ||  45.  || 38
     * Schreibt den String in das File
     */

    QFile file(str);
    QString string_Tool;
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream textStream(&file);
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
    }
}
