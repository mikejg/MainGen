#ifndef PROJECT_LOADER_H
#define PROJECT_LOADER_H

#include <QObject>

#include <QFileDialog>
#include <QWidget>
#include "dbManager.h"
#include "mfile.h"
#include "project.h"

class Project_Loader : public QObject
{
    Q_OBJECT
private:
    DBManager* dbManager;
    MFile* mfile;

public:
    explicit Project_Loader(QObject *parent = nullptr);

    bool add_Project(QString, Project*);
    bool load_Project(QString, Project*);
    void set_DBManager(DBManager* dbm) {dbManager = dbm;}
signals:
    void sig_Log(QString);
    void sig_Err(QString);
};

#endif // PROJECT_LOADER_H
