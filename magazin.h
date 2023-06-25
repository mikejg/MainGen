#ifndef MAGAZIN_H
#define MAGAZIN_H

#include <QObject>
#include <dbManager.h>
#include "mfile.h"
#include "toollist.h"
class Magazin : public QObject
{
    Q_OBJECT
private:
    DBManager* dbManager;
    MFile*     mfile;
    ToolList*  toolList;

public:
    explicit Magazin(QObject *parent = nullptr);

    bool      contains(Tool*);
    bool      create_ToolList();
    int       get_Size(){return toolList->getSize();}
    ToolList* get_ToolList() {return toolList;}
    void      set_DBManager(DBManager* dbm) {dbManager = dbm;}
    void      set_FilePath(QString str){mfile->setFileName(str);}

signals:
    void sig_Err(QString);
    void sig_Log(QString);
};

#endif // MAGAZIN_H
