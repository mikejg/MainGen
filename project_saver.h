#ifndef PROJECTSAVER_H
#define PROJECTSAVER_H

#include <QObject>
#include <QTextStream>
#include <QTimer>

#include "dbManager.h"
#include "dialogProgress.h"
#include "mfile.h"
#include "parser_placeholder.h"
#include "parser_programm.h"
#include "project.h"
#include "settings.h"

class Project_Saver : public QObject
{
    Q_OBJECT
private:
    enum Marker {KopfStart, KopfEnd, keinMarker};

    int                 counter;
    DBManager*          dbManager;
    DialogProgress*     dialogProgress;
    Marker              marker;
    MFile*              mfile;
    Parser_PlaceHolder* parser_PlaceHolder;
    Parser_Programm*    parser_Programm;
    Project*            project;
    bool                bool_Repetition;
    Settings*           settings;
    QString             string_Destination;
    QString             string_Programm;
    QString             string_ProgrammDir;
    QString             string_ProjectClamping;
    QString             string_ProjectFullName;
    QString             string_ProjectName;
    QString             string_Source;
    QStringList         stringList_Content_MainProgramm;
    QStringList         stringList_Programme;
    QTimer*             timer;

    QString formatString(QString);
    void    init_Saving();
    bool    finish_Cleaning();
    bool    finish_BlankControl();
    bool    finish_Load();
    bool    finish_Touch_Sp1();


public:
    explicit Project_Saver(QObject *parent = nullptr);

    bool save_Rustplan(QString, ToolList*);
    void set_DBManager(DBManager* dbm) {dbManager = dbm;}
    void set_DialogProgress(DialogProgress* dw) {dialogProgress = dw;}
    void set_Project(Project* p);
    void set_Settings(Settings* s) {settings = s;}

public slots:
    void slot_nextProgramm();
    void slot_Save_Project(bool);

signals:
    void sig_Err(QString);
    void sig_Log(QString);
};

#endif // PROJECTSAVER_H
