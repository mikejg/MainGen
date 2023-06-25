#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
private:
    QSettings*  settings;
    QString     string_ProgrammDir;
    QString     string_MagazinDir;
    QString     string_WerkzeugDB;
    bool        bool_Numbering;
    bool        bool_AufmassMax;
    int         int_WerkzeugPlatze;

public:
    explicit Settings(QObject *parent = nullptr);

    //get Funktionen
    QString get_ProgrammDir()    {return settings->value("ProgrammDir", "").toString();}
    QString get_MagazinDir()     {return settings->value("MagazinDir", "").toString();}
    QString get_WerkzeugDB()     {return settings->value("WerkzeugDB", "").toString();}
    bool    get_Numbering()      {return settings->value("Nummerierung",true).toBool();}
    bool    get_AufmassMax()     {return settings->value("AufmassMax",false).toBool();}
    int     get_WerkzeugPlatze() {return settings->value("WerkzeugPlatze",0).toInt();}
    QString get_NextCheck()      {return settings->value("NextCheck","").toString();}

    //set Funktionen
    void set_ProgrammDir(QString str)   {settings->setValue("ProgrammDir", str);}
    void set_MagazinDir(QString str)    {settings->setValue("MagazinDir", str);}
    void set_WerkzeugDB(QString str)    {settings->setValue("WerkzeugDB", str);}
    void set_Numbering(QString str)     {settings->setValue("Nummerierung",str);}
    void set_AufmassMax(QString str)    {settings->setValue("AufmassMax",str);}
    void set_WerkzeugPlatze(int p)      {settings->setValue("WerkzeugPlatze",p);}
    void set_NextCheck(QString str)     {settings->setValue("NextCheck",str);}
signals:

};

#endif // SETTINGS_H
