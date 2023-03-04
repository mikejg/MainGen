#ifndef PARSER_WZM_H
#define PARSER_WZM_H

#include <QObject>
#include <QObject>
#include <QMap>
#include <QDebug>
#include <QFile>
#include "toollist.h"
#include "dbManager.h"

class Parser_WZM : public QObject
{
    Q_OBJECT
private:
    QStringList stringList_Lines;
    QFile file_fileToParse;
    DBManager* dbManager;

    bool openFile();
    void readFile();
    void splitLine(ToolList*);
    void checkParted(ToolList*);

public:
    explicit Parser_WZM(QObject *parent = nullptr);
    void     parse(QString, ToolList*);
    void     checkPartedTools(QString, ToolList*);
    void    setDBManager(DBManager* dbm) {dbManager = dbm;}
signals:
    void sig_Log(QString);
    void sig_Err(QString);
};

#endif // PARSER_WZM_H
