#ifndef PARSER_PLACEHOLDER_H
#define PARSER_PLACEHOLDER_H

#include <QObject>
#include <QStringList>
#include "mfile.h"
#include "project.h"

class Parser_PlaceHolder : public QObject
{
    Q_OBJECT
private:
    MFile* mfile;

    bool writeG55(QStringList* stringList_Content, Project*);

public:
    explicit Parser_PlaceHolder(QObject *parent = nullptr);

    QStringList set_PlaceHolder_Antasten(QStringList, Project*);
    QStringList set_PlaceHolder_Cleaning(QStringList, Project*);
    QStringList set_PlaceHolder_BlankControl(QStringList, Project*);
    QStringList set_PlaceHolder_MainProgramm(QStringList, Project*);

signals:
    void sig_Err(QString);
    void sig_Log(QString);
};

#endif // PARSER_PLACEHOLDER_H
