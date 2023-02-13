#include "holder.h"

Holder::Holder(QObject *parent) : QObject(parent)
{
    counter = 0;
    string_Name          = " ";
    string_Comment       = " ";
}

void Holder::clear()
{
    string_Name = " ";
    string_Comment = " ";

    counter = 0;
}
