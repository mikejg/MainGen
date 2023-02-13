#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>
#include <QDebug>
#include <QFile>
#include "toollist.h"
#include "holder.h"
#include "dbManager.h"

class Parser : public QObject
{
    Q_OBJECT

private:
    QStringList stringList_Lines;
    QFile file_fileToParse;
    QString string_Ruestplan;
    DBManager* dbManager;

    bool openFile();
    void readFile();
    void splitLine(ToolList*);

public:
    explicit Parser(QObject *parent = nullptr);
    void     parse(QString, ToolList*);
    void     save(QString, ToolList*);
    void    setDBManager(DBManager* dbm) { dbManager = dbm; }
signals:
    void sig_Log(QString);
    void sig_Err(QString);

};

#endif // PARSER_H
