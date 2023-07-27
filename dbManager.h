#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QDir>
#include "dialogProgress.h"
#include "tool.h"
#include "toollist.h"
#include "mfile.h"

class DBManager : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase m_db;
    QString string_WerkzeugDB;
    QString string_MainDB;
    QString string_Log;
    QStringList stringList_Rustplane;
    QStringList stringList_Split;
    DialogProgress* dialogProgress;
    QString string_Project;
    QString string_WiederholFertigung;
    ToolList* toolList;
    QTimer* timer;
    bool openMainDB();
    bool openWerkzeugDB();
    int counter;
    MFile* mfile;
    bool parse_Rustplan(QString string_FileName);

public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

    void    clearDB();

    QString getAusspannLaenge(QString toolID);
    QString getFreistellLaenge(QString toolID);
    QString getHolder(QString toolID);
    QString getDescription(QString toolID);
    int     getCounter(QString toolID);
    void    getTop100(ToolList*);
    int     getWiederholFertigung(QString);

    QString insertRuestplan(QString);
    void    insertTool(Tool*, QString, QString);
    QString insertTool (Tool*);
    QString insertHolder(QString);
    void    insertProject(QString, ToolList*);
    void    open();
    void    restore();
    void    set_DialogProgress(DialogProgress* dp) {dialogProgress = dp;}

signals:
    void sig_Log(QString);
    void sig_Err(QString);

public slots:
    void slot_InsertRPL();
};

#endif // DBMANAGER_H
