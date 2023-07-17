#include "project_loader.h"

Project_Loader::Project_Loader(QObject *parent)
    : QObject{parent}
{
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));
}

bool Project_Loader::add_Project(QString string_FileName, Project* project)
{
    QStringList stringList_Split;
    QString string_TNumber;
    QStringList stringList_Parts;
    Tool* tool = new Tool(this);
    ToolList* toolList = project->get_ToolList();


    QFileInfo fileInfo = QFileInfo(string_FileName);
    stringList_Split = fileInfo.baseName().split("_");
    if(stringList_Split.size() < 4)
    {
        emit sig_Err("----------------------------------------------------------------");
        emit sig_Err("|                      Falscher DateiName                      |");
        emit sig_Err("----------------------------------------------------------------");
        emit sig_Err("| Zeichnungsnummer_Zeichnungsstand_Spannung_Wiederholfertigung |");
        emit sig_Err("| E14207809_E02_Sp2_0                                          |");
        emit sig_Err("| Beheben Sie das Problem und starten Sie die Applikation neu  |");
        emit sig_Err("----------------------------------------------------------------");
        return false;
    }

    mfile->setFileName(string_FileName);
    if(!mfile->read_Content())
        return false;

    foreach(QString string_Line, mfile->get_Content())
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
            toolList->insert_Tool(tool, true);
            //sig_Log(Q_FUNC_INFO + string_TNumber + QString(" | ") + tool->get_HalterName());
        }
    }
    toolList->sort_ID();
    project->logProjectData();

    return true;
}

bool Project_Loader::load_Project(QString string_FileName, Project* project)
{
    qDebug() << Q_FUNC_INFO;
    QStringList stringList_Split;
    QString string_TNumber;
    QStringList stringList_Parts;
    Tool* tool = new Tool(this);
    ToolList* toolList = project->get_ToolList();


    QFileInfo fileInfo = QFileInfo(string_FileName);
    stringList_Split = fileInfo.baseName().split("_");
    if(stringList_Split.size() < 4)
    {
        emit sig_Err("----------------------------------------------------------------");
        emit sig_Err("|                      Falscher DateiName                      |");
        emit sig_Err("----------------------------------------------------------------");
        emit sig_Err("| Zeichnungsnummer_Zeichnungsstand_Spannung_Wiederholfertigung |");
        emit sig_Err("| E14207809_E02_Sp2_0                                          |");
        emit sig_Err("| Beheben Sie das Problem und starten Sie die Applikation neu  |");
        emit sig_Err("----------------------------------------------------------------");
        return false;
    }

    project->set_ProjectName(stringList_Split.at(0));
    project->set_ProjectStatus(stringList_Split.at(1));
    project->set_ProjectClamping(stringList_Split.at(2));
    project->set_ProjectFullName();
    project->set_RepetitiveManufacturing();

    toolList->clear();

    mfile->setFileName(string_FileName);
    if(!mfile->read_Content())
        return false;

    foreach(QString string_Line, mfile->get_Content())
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
    toolList->sort_ID();
    project->logProjectData();

    return true;
}
