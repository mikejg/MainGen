#ifndef PARSER_PROGRAMM_H
#define PARSER_PROGRAMM_H

#include <QObject>
#include <dbManager.h>
#include "mfile.h"
#include "toollist.h"

class Parser_Programm : public QObject
{
    Q_OBJECT

private:
    DBManager* dbManager;
    MFile* mfile;
    enum Marker {kein_Marker, werkzeugliste_Anfang, werkzeugliste_Ende, Messerkopf, neuesWerkzeug};

    Marker marker;

    void insertTool(QString, ToolList*);
public:
    explicit Parser_Programm(QObject *parent = nullptr);
    QString  parseProjectName(QString);
    bool     parseProjectTool(QString, ToolList*);
    void     setDBManager(DBManager* dbm) {dbManager = dbm;}
signals:
    void sig_Log(QString);
    void sig_Err(QString);

};

#endif // PARSER_PROGRAMM_H
