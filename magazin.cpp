#include "magazin.h"

Magazin::Magazin(QObject *parent)
    : QObject{parent}
{
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));

    toolList = new ToolList(this);
}

bool Magazin::contains(Tool* tool)
{
    if(!toolList->contains(tool))
        return false;
    return true;
}
bool Magazin::create_ToolList()
{
    if(!mfile->read_Content())
        return false;
    toolList->clear();
    QString string_ToolID;
    Tool* tool = new Tool(this);
    foreach(QString string_Line, mfile->get_Content())
    {
        /* Wenn string_Line mit $TC_TP2 startet
         * schneide solage von Links ein Zeichen ab bis du auf ein " stösst
         * entferne am ende das "
         * schreibe den übrig gebliebenen string_Line in string_ToolID
         * */
        if(string_Line.startsWith("$TC_TP2"))
        {
            while(!string_Line.startsWith("\"") && string_Line.size() > 0)
            {
                string_Line = string_Line.right(string_Line.length()-1);
            }
            string_Line.remove("\"");
            string_ToolID=string_Line;
            //continue;
        }

        /* Wenn string_Line mit $A_TOOLMN startet
         * schneide solage von Links ein Zeichen ab bis du auf ein = stösst
         * entferne am Ende das =
         * Wenn string_Line nicht leer ist und string_Line nicht 0 ist
         * erstelle das Tool und schreibe ist in die toolList */
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
                toolList->insert_Tool(tool, false);
            }
        }
    }
    return true;
}
