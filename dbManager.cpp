#include "dbManager.h"
#include <QSqlQuery>
#include <QSqlError>

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    string_WerkzeugDB = QDir::homePath() + "/MainGen/WerkzeugDB/WerkzeugDB.db";
    string_MainDB = QDir::homePath() + "/MainGen/mainGenDB.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(string_WerkzeugDB);
}

DBManager::~DBManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

void DBManager::open()
{
    if (m_db.isOpen())
    {
        m_db.close();
        qDebug() << "close database";
    }
    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

void DBManager::clearDB()
{
    m_db.close();
    QFile dbFile(QDir::homePath() + "/MainGen/mainGenDB.db");
    dbFile.remove();
    QFile::copy(QDir::homePath() + "/MainGen/mainGenDB_leer.db",QDir::homePath() + "/MainGen/mainGenDB.db");
    return;
}

void DBManager::getTop100(ToolList* toolList)
{
    toolList->clear();
    Tool* tool;

    if(!openMainDB())
        return;

    /*  select T_Number, counter from NCTools order by counter desc, T_Number asc limit 100; */
    //QSqlQuery query("select * From NCTools where counter > 1 order by T_Number;");

    QSqlQuery query("select T_Number, counter, Description from NCTools order by counter desc, T_Number asc limit 100;");
    int i = 0;
    while (query.next() && i < 100)
    {
        tool = new Tool(toolList);
        tool->set_Number(query.value("T_Number").toString());
        tool->set_Description(query.value("Description").toString());
        tool->set_counter(query.value("counter").toInt());

        toolList->insert_Tool(tool);
        i++;
    }
}

QString DBManager::getDescription(QString toolID)
{
    if(!openWerkzeugDB())
        return "";
    QString string_Description;

    QSqlQuery query("select nc_name from NCTools "
                    "where nc_number_str = '" + toolID + "';");

    //qDebug() << query.lastQuery();
    //qDebug() << query.lastError().text();

    while (query.next())
    {
        string_Description = query.value("nc_name").toString();
    }
    return string_Description;
}

int DBManager::getCounter(QString toolID)
{
    if(!openMainDB())
        return 0;
    int counter = 0;

    QSqlQuery query("select counter from NCTools "
                    "where T_Number = '" + toolID + "';");

    //qDebug() << query.lastQuery();
    //qDebug() << query.lastError().text();

    while (query.next())
    {
        counter = query.value("counter").toInt();
        //qDebug() << QString("%1").arg(counter);
    }

    //qDebug() << QString("%1").arg(counter);
    return counter;
}

QString DBManager::getAusspannLaenge(QString toolID)
{
    if(!openWerkzeugDB())
        return "";
    QString string_AusspannLaenge;
    string_AusspannLaenge = "0";

    QSqlQuery query("select nc_number_str, tool_length from NCTools where nc_number_str = '" + toolID + "';");

    //qDebug() << query.lastQuery();
    //qDebug() << query.lastError().text();

    while (query.next())
    {
        string_AusspannLaenge = query.value("tool_length").toString();
    }
    return string_AusspannLaenge;
}

QString DBManager::getFreistellLaenge(QString toolID)
{
    if(!openWerkzeugDB())
        return "";
    QString string_FreistellLaenge;
    bool bool_Hals = true;

    string_FreistellLaenge = "0";

    QSqlQuery query("SELECT Tools.dbl_param5, Tools.bool_param2 from NCTools "
                    "INNER JOIN Tools on "
                    "Tools.id = NCTools.tool_id "
                    "where NCTools.nc_number_str = '" + toolID + "';");

    //qDebug() << query.lastQuery();
    //qDebug() << query.lastError().text();

    while (query.next())
    {
        string_FreistellLaenge = query.value("dbl_param5").toString();
        bool_Hals = query.value("bool_param2").toBool();
    }

    if(!bool_Hals)
        string_FreistellLaenge = " - ";

    return string_FreistellLaenge;
}

QString DBManager::getHolder(QString toolID)
{
    if(!openWerkzeugDB())
        return "";

    //return "";

    QString string_Holder;

    QSqlQuery query("SELECT Holders.name from NCTools "
                    "INNER JOIN Holders on "
                    "Holders.id = NCTools.holder_id "
                    "where NCTools.nc_number_str = '" + toolID + "';");

    /*
    string_Log = Q_FUNC_INFO + QString(" " + query.lastQuery());
    sig_Err(string_Log);
    string_Log = Q_FUNC_INFO + QString(" " + query.lastError().text());
    sig_Err(string_Log);
    */
    while (query.next())
    {
        string_Holder = query.value("name").toString();
        //string_Log = Q_FUNC_INFO + QString(" query.next() string_Holder = " +string_Holder);
        //sig_Err(string_Log);
    }
    return string_Holder;
}

QString DBManager::insertRuestplan(QString string_Ruestplan)
{
    /*Öffnet Hauptdatenbank*/
    if(!openMainDB())
        return "";

    QString string_RuestplanId;

    /*Suche in der Datenbank nach dem Rüstplan
     * wenn der Rüstplan schon in der Datenbank existiert,
     * wird die ID zurückgeben*/
    QSqlQuery query("SELECT id, name from Ruestplan "
                    "where name = '" + string_Ruestplan + "';");

    while (query.next())
    {
        string_RuestplanId = query.value("id").toString();
    }

    if(!string_RuestplanId.isEmpty())
        return string_RuestplanId;

    /*Erstelle in der Tabelle Ruestplan einen neuen Eintrag*/
    string_Log = Q_FUNC_INFO + QString(": keine Rüstplan ID gefunden") ;
    emit sig_Log(string_Log);

    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO Ruestplan (name) VALUES (:name)");
    queryAdd.bindValue(":name", string_Ruestplan);

    if(queryAdd.exec())
    {
        string_Log = Q_FUNC_INFO + QString(" Ruestplan: " + string_Ruestplan + " eingefügt");
        emit sig_Log(string_Log);
    }
    else
    {
        string_Log = Q_FUNC_INFO + QString(" Ruestplan: " + string_Ruestplan + " einfügen fehlgeschlagen");
        emit sig_Err(string_Log);
        return "";
    }

    /*Suche nochmal in der Datenbank nach dem Rüstplan und gib die ID zurück*/
    query.exec();
    while (query.next())
    {
        string_RuestplanId = query.value("id").toString();
    }

    return string_RuestplanId;
}

void DBManager::insertTool(Tool* tool, QString string_RuestplanID, QString string_RuestplanName)
{
    QString string_ToolID;
    QString string_ZuordnungID;

    /*Öffnet Hauptdatenbank*/
    if(!openMainDB())
        return;

    /*Trage das Tool in die Datenbank ein und gibt die ID zurück*/
    string_ToolID = insertTool(tool);

    /* Suche in der Zuordnungs Tabelle nach einem Eintrag aus
     * Tool_ID und Ruestplan_ID
     * Wenn ein Eintrag existiert beende die Funktion*/
    QSqlQuery query("SELECT id from NCTools_Ruestplan "
                    "where nctool_id = " +
                     string_ToolID +
                    " AND ruestplan_id = " +
                    string_RuestplanID + ";");

    while (query.next())
    {
        string_ZuordnungID= query.value("id").toString();
    }

    if(!string_ZuordnungID.isEmpty())
        return;

    /*Erstelle einen neuen Eintrag in der Zuordnungstabelle NCTools_Ruestplan*/
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO NCTools_Ruestplan (nctool_id, T_Number, ruestplan_id, Ruestplan) "
                                          "VALUES (:nctool_id, "
                                                  ":T_Number, "
                                                  ":ruestplan_id, "
                                                  ":Ruestplan )");
    queryAdd.bindValue(":nctool_id", string_ToolID);
    queryAdd.bindValue(":T_Number", tool->get_Number());
    queryAdd.bindValue(":ruestplan_id", string_RuestplanID);
    queryAdd.bindValue(":Ruestplan", string_RuestplanName);

    if(queryAdd.exec())
    {
        string_Log = Q_FUNC_INFO + QString(" " + tool->get_Number() + " - " + string_RuestplanName + " eingefügt");
        emit sig_Log(string_Log);
    }
    else
    {
        string_Log = Q_FUNC_INFO + QString(" Zuordung fehlgeschlagen: ");
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastQuery());
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastError().text());
        emit sig_Err(string_Log);
        return;
    }

    /*Erhöhe den Zähler der Werkzeugs*/
    query.prepare("Update NCTools Set counter = counter + 1 "
                  "Where id = " + string_ToolID + ";");

    if(query.exec())
    {
        //qDebug() << tool->get_Number() << " Zähler erhöht";
    }
    else
    {
        string_Log = Q_FUNC_INFO + QString(" Erhoehe Zaehler fehlgeschlagen: ");
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastQuery());
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastError().text());
        emit sig_Err(string_Log);
        return;
    }
}

QString DBManager::insertTool(Tool* tool)
{
     /*Öffnet Hauptdatenbank*/
    if(!openMainDB())
        return "";

    QString string_ToolID;
    QString string_HolderID;

    /*Suche nach dem Tool in der Datenbank,
     * wenn das Tool schon in der Datenbank existiert,
     * wird die ID zurückgeben*/
    QSqlQuery query("SELECT id, T_Number from NCTools "
                    "where T_Number = '" +
                    tool->get_Number()+ "';");

    while (query.next())
    {
        string_ToolID = query.value("id").toString();
    }
    //qDebug() << query.lastQuery();

    if(!string_ToolID.isEmpty())
        return string_ToolID;


    /*Trägt den Halter in die DatenBank ein und gibt die ID zurück*/
    string_HolderID = insertHolder(tool->get_HalterName());


    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO NCTools (T_Number, Description, GesamtLaenge, AusspannLaenge, FreistellLaenge, "
                                          "counter, holder_id) "
                                          "VALUES (:T_Number, "
                                                  ":Description, "
                                                  ":GesamtLaenge, "
                                                  ":AusspannLaenge, "
                                                  ":FreistellLaenge, "
                                                  ":counter, "
                                                  ":holder_id)");
    queryAdd.bindValue(":T_Number", tool->get_Number());
    queryAdd.bindValue(":Description", tool->get_Description());
    queryAdd.bindValue(":GesamtLaenge", tool->get_ToolGL());
    queryAdd.bindValue(":AusspannLaenge", tool->get_ToolAL());
    queryAdd.bindValue(":FreistellLaenge", tool->get_ToolFL());
    queryAdd.bindValue(":counter", 0);
    queryAdd.bindValue(":holder_id", string_HolderID);
    if(queryAdd.exec())
    {
        //qDebug() << "Tool: " << tool->get_Number() << " eingefügt";
    }
    else
    {
        string_Log = Q_FUNC_INFO + QString(" Tool einfuegen fehlgeschlagen: ");
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastQuery());
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastError().text());
        emit sig_Err(string_Log);
        return "";
    }


    query.exec();
    while (query.next())
    {
        string_ToolID = query.value("id").toString();
    }

    return string_ToolID;
}

QString DBManager::insertHolder(QString string_HolderName)
{
    QString string_HolderID;
    /*Öffnet Hauptdatenbank*/
    if(!openMainDB())
        return "";

    /*Suche nach dem Halter in der Datenbank,
     * wenn der Halter schon in der Datenbank existiert,
     * wird die ID zurückgeben*/
    QSqlQuery query("SELECT id, name from Holders "
                    "where name = '" + string_HolderName + "';");

    while (query.next())
    {
        string_HolderID = query.value("id").toString();
    }

    if(!string_HolderID.isEmpty())
        return string_HolderID;

    //qDebug() << "keine Halter gefunden" ;
    /*Erstelle in der Tabelle Holders einen neuen Eintrag*/
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO Holders (name) VALUES (:name)");
    queryAdd.bindValue(":name", string_HolderName);

    if(queryAdd.exec())
    {
        //qDebug() << "Halter: " << string_HolderName << " eingefügt";
    }
    else
    {
        string_Log = Q_FUNC_INFO + QString(" Halter einfuegen fehlgeschlagen: ");
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastQuery());
        emit sig_Err(string_Log);
        string_Log = Q_FUNC_INFO + QString(" " + queryAdd.lastError().text());
        emit sig_Err(string_Log);
        return "";
    }

    /*Suche nochmal in der Datenbank nach dem Halter und gib die ID zurück*/
    query.exec();
    while (query.next())
    {
        string_HolderID = query.value("id").toString();
    }
    return string_HolderID;
}

bool DBManager::openMainDB()
{   
    /*Wenn der DatenBank Name gleich dem String string_MainDB und
     * die Datenbank schon offen ist, dann gebe true zurück*/
    if(m_db.databaseName() == string_MainDB && m_db.isOpen())
        return true;

    /*Schliesse die DatenBank
     * Setze den DatenBanke Namen auf string_MainDB
     * Öffne die Datenbank
     * gib true zurück */
    m_db.close();
    m_db.setDatabaseName(string_MainDB);
    if(!m_db.open())
    {
       sig_Err(Q_FUNC_INFO + QString(" " + m_db.databaseName() + " konnte nicht geöffnet werden"));
       return false;
    }
    //sig_Log(Q_FUNC_INFO + QString(" " + m_db.databaseName() + " erfolgreich geöffnet"));
    return true;
}

bool DBManager::openWerkzeugDB()
{
    /*
    bool bool_return = true;

    if(m_db.databaseName() == string_MainDB)
    {
        m_db.close();
        m_db.setDatabaseName(string_WerkzeugDB);

        if (!m_db.open())
        {
            qDebug() << "Error: connection with WerkzeugDB fail";
            bool_return = false;
        }
        else
        {
            qDebug() << "Database: connection with WerkzeugDB ok";
        }
    }

    if(m_db.databaseName() == string_WerkzeugDB && !m_db.isOpen())
    {
        sig_Err(Q_FUNC_INFO + QString(" " + m_db.databaseName() + " NICHT OFFEN"));
        if(!m_db.open())
            sig_Err(Q_FUNC_INFO + QString(" " + m_db.databaseName() + " zweiter Versuch fehlgeschlagen"));
    }
    return bool_return;
    */

    if(m_db.databaseName() == string_WerkzeugDB && m_db.isOpen())
        return true;

    m_db.close();
    m_db.setDatabaseName(string_WerkzeugDB);
    if(!m_db.open())
    {
       sig_Err(Q_FUNC_INFO + QString(" " + m_db.databaseName() + " konnte nicht geöffnet werden"));
       return false;
    }
    //sig_Log(Q_FUNC_INFO + QString(" " + m_db.databaseName() + " erfolgreich geöffnet"));
    return true;
}

void DBManager::insertProject(QString string_RuestplanName, ToolList* toolList)
{
    /*Öffnet die Hauptdatenbank*/
    if(!openMainDB())
        return;

    /*Rüstplan wir in der Datenbank erstellt und die ID zurückgegeben*/
    QString string_RuestplanID = insertRuestplan(string_RuestplanName);

    /*Trage alle Werkzeuge aus der ToolList in die Datenbank ein*/
    foreach(Tool* tool, toolList->getList())
    {
        insertTool(tool, string_RuestplanID, string_RuestplanName);
    }
}

int DBManager::getWiederholFertigung(QString string_Projekt)
{
    qDebug() << Q_FUNC_INFO;
    int int_Return;
    /*Öffnet die Hauptdatenbank*/
    if(!openMainDB())
        return -1;

    QSqlQuery query("select name from Ruestplan where name like '" + string_Projekt + "%';");
    qDebug() << query.lastQuery();

    //QSqlQuery query("select name from Ruestplan;");

    int_Return = 0;
    while(query.next())
    {
        int_Return++;
    }
    return int_Return;
}
