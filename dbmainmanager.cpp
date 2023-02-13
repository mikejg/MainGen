#include "dbMainManager.h"

dbMainManager::dbMainManager(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(QDir::homePath() + "/MainGen/mainGenDB.db");
}

void dbMainManager::open()
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


void dbMainManager::insertTool(QString string_Ruestplan, Tool* tool)
{

}

QString dbMainManager::insertRuestplan(QString string_Ruestplan)
{
    QString string_RuestplanId;

    QSqlQuery query("SELECT id, name from Ruestplan "
                    "where name = '" + string_Ruestplan + "';");

    while (query.next())
    {
        string_RuestplanId = query.value("id").toString();
    }

    if(!string_RuestplanId.isEmpty())
        return string_RuestplanId;

    qDebug() << "keine Rüstplan ID gefunden" ;
}
