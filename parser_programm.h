#ifndef PARSER_PROGRAMM_H
#define PARSER_PROGRAMM_H

#include <QObject>
#include <QFileInfo>
#include "dbManager.h"
#include "mfile.h"
#include "toollist.h"

class Parser_Programm : public QObject
{
    Q_OBJECT

private:
    enum Marker {kein_Marker, werkzeugliste_Anfang, werkzeugliste_Ende, Messerkopf, neuesWerkzeug};
    bool                   bool_ersterBruch;
    bool                   bool_Messerkopf;
    bool                   bool_EinlippenBohrer;
    DBManager*             dbManager;
    QFileInfo              fileInfo_Source;
    QFileInfo              fileInfo_Target;
    Marker                 marker;
    QMap<QString, QString> map_Bruch;
    MFile*                 mfile;
    QString                string_Line;
    QStringList            stringList_Content;


    void compare(QStringList, QStringList);
    void insertTool(QString, ToolList*);
public:
    explicit Parser_Programm(QObject *parent = nullptr);

    void     finish(QString);
    bool     loadBruch();
    QString  parseProjectName(QString);
    bool     parseProjectTool(QString, ToolList*);
    void     setDBManager(DBManager* dbm) {dbManager = dbm;}

signals:
    void sig_Log(QString);
    void sig_Err(QString);

};

#endif // PARSER_PROGRAMM_H
