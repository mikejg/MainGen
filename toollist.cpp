#include "toollist.h"
#include <QDebug>

ToolList::ToolList(QObject *parent)
    : QObject{parent}
{

}

void ToolList::insert_Tool(Tool* tool)
{
    bool match = false;
    int i = 0;
    Tool* t;

    //Suche in der Liste ob es das Tool schon gibt [und erhöhe gegebenen Falls den Zähler]
    while(i<list.size() && !match)
    {
        if(list.at(i)->get_Number() == tool->get_Number())
        {
            //list.at(i)->inc_counter();
            match = true;
        }
        i++;
    }


    if(!match)
    {
       t = new Tool(this);
       t->set_Number(tool->get_Number());
       t->set_Description(tool->get_Description());
       t->set_ToolGL(tool->get_ToolGL());
       t->set_ToolAL(tool->get_ToolAL());
       t->set_ToolFL(tool->get_ToolFL());
       t->set_HalterName(tool->get_HalterName());
       t->set_counter(tool->get_counter());
       list.append(t);
    }

    return;
}

void ToolList::prepend(Tool* tool)
{
    Tool* t = new Tool(this);
    t->set_Number(tool->get_Number());
    t->set_Description(tool->get_Description());
    t->set_ToolGL(tool->get_ToolGL());
    t->set_ToolAL(tool->get_ToolAL());
    t->set_ToolFL(tool->get_ToolFL());
    list.prepend(t);
}

/*void ToolList::sort_Top100()
{

    //Nach Häufigkeit sortieren
    for (int i = 0; i<list.size(); i++)
    {

        for(int j = 0; j<i; j++)
        {
            if(list.at(i)->get_counter() > list.at(j)->get_counter())
            {
                list.move(i,j);
                break;
            }
        }
    }


    //Nur die ersten 100 behalten
    for (int i = 0; i<list.size(); i++)
    {
        if(i > 100)
        {
            list.removeOne(list.at(i));
            i--;
        }
    }

    //Was übrig ist nach der ID sortieren
    for (int i = 0; i<list.size(); i++)
    {

        for(int j = 0; j<i; j++)
        {
            if(list.at(i)->get_Number() < list.at(j)->get_Number())
            {
                list.move(i,j);
                break;
            }
        }
    }


}*/

void ToolList::sort_ID()
{
    for (int i = 0; i<list.size(); i++)
    {

        for(int j = 0; j<i; j++)
        {
            if(list.at(i)->get_Number() < list.at(j)->get_Number())
            {
                list.move(i,j);
                break;
            }
        }
    }
}

void ToolList::sort_Counter()
{
    for (int i = 0; i<list.size(); i++)
    {

        for(int j = 0; j<i; j++)
        {
            if(list.at(i)->get_counter() < list.at(j)->get_counter())
            {
                list.move(i,j);
                break;
            }
        }
    }
}

bool ToolList::contains(Tool* t)
{
    //qDebug() << "ToolList::contains: " + t->get_Number();
    foreach(Tool* tool, list)
    {
        if(tool->get_Number() == t->get_Number())
        {
            //qDebug() << "Tool: " + t->get_Number() + " = " + tool->get_Number();
            return true;
        }
    }
    //qDebug() << "Tool: " + t->get_Number();
    return false;
}

void ToolList::set_Description(ToolList* tl)
{
    QList<Tool*> list_Source = tl->getList();
    for(int i = 0; i < list.size(); i++)
    {
        for(int j = 0; j< list_Source.size(); j++)
        {
            if(list.at(i)->get_Number() == list_Source.at(j)->get_Number())
                list.at(i)->set_Description(list_Source.at(j)->get_Description());
        }
    }
}
