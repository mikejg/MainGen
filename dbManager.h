#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QDir>
#include "tool.h"
#include "toollist.h"

class DBManager : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase m_db;
    QString string_WerkzeugDB;
    QString string_MainDB;
    QString string_Log;
    bool openMainDB();
    bool openWerkzeugDB();

public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();
    void    open();
    void    clearDB();
    QString getAusspannLaenge(QString toolID);
    QString getFreistellLaenge(QString toolID);
    QString getHolder(QString toolID);
    QString getDescription(QString toolID);
    int     getCounter(QString toolID);
    QString insertRuestplan(QString);
    void    insertTool(Tool*, QString, QString);
    QString insertTool (Tool*);
    QString insertHolder(QString);
    void    insertProject(QString, ToolList*);
    void    getTop100(ToolList*);
    int     getWiederholFertigung(QString);

signals:
    void sig_Log(QString);
    void sig_Err(QString);
};

#endif // DBMANAGER_H
