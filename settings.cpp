#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    settings = new QSettings("Gareiss", "MainGen");
    string_ProgrammDir = settings->value("ProgrammDir", "").toString();
    string_MagazinDir  = settings->value("MagazinDir", "").toString();
    string_WerkzeugDB  = settings->value("WerkzeugDB", "").toString();
    bool_Numbering     = settings->value("Nummerierung",true).toBool();
    bool_AufmassMax    = settings->value("AufmassMax",false).toBool();
    int_WerkzeugPlatze = settings->value("WerkzeugPlatze").toInt();
}
