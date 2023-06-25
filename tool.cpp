#include "tool.h"

Tool::Tool(QObject *parent)
    : QObject{parent}
{
    counter = 0;
    number          = " ";
    description     = " ";
    gesammtLaenge   = " ";
    ausspannLaenge  = " ";
    freistellLaenge = " ";
    halterComment   = " ";
    halterName      = " ";
}

void Tool::clear()
{
    number = " ";
    description = " ";
    gesammtLaenge = " ";
    ausspannLaenge = " ";
    freistellLaenge = " ";
    halterComment   = " ";
    halterName      = " ";

    counter = 0;
}
