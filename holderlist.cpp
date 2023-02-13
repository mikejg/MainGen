#include "holderlist.h"

HolderList::HolderList(QObject *parent) : QObject(parent)
{

}

void HolderList::insert_Holder(Holder* holder)
{
    bool match = false;
    int i = 0;
    Holder* h;

    //Suche in der Liste ob es den Halter schon gibt und erhöhe gegebenen Falls den Zähler
    while(i<list.size() && !match)
    {
        if(list.at(i)->get_Name() == holder->get_Name())
        {
            list.at(i)->inc_counter();
            match = true;
        }
        i++;
    }

    if(!match)
    {
       h = new Holder(this);
       h->set_Name(holder->get_Name());
       h->set_Comment(holder->get_Comment());
       h->inc_counter();
       list.append(h);
       //qDebug() << "ToolList::insertTool " << tool->get_Number();
    }

    return;
}
