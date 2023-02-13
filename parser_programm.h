#ifndef PARSER_PROGRAMM_H
#define PARSER_PROGRAMM_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTextEdit>
#include "toollist.h"
#include "dbManager.h"

class Parser_Programm : public QObject
{
    Q_OBJECT

private:
    QStringList stringList_Lines;
    QFile file_fileToParse;
    QFileInfo fileInfo;
    QFileInfo fileInfo_Source;
    QFileInfo fileInfo_Target;
    QTextEdit* textEdit_Log;
    DBManager* dbManager;

    enum Marker {kein_Marker, werkzeugliste_Anfang, werkzeugliste_Ende, Messerkopf};
    bool bool_ersterBruch;
    bool bool_Messerkopf;
    bool bool_EinlippenBohrer;

    Marker marker;

    void insertTool(QString, ToolList*);
    bool openFile();
    void readFile();
    void findTool(ToolList*);
    void finish();
    void saveFile();
    void compare();

public:
    explicit Parser_Programm(QObject *parent = nullptr);
    void     parse(QString, ToolList*);
    QString  parseProjectName(QString);
    void    finish(QString stringFile);
    void    setTextEdit (QTextEdit* te) { textEdit_Log = te;}
    void    setDBManager(DBManager* dbm) { dbManager = dbm; }

signals:
    void sig_Log(QString);
    void sig_Err(QString);

};

#endif // PARSER_PROGRAMM_H
