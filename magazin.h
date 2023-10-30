#ifndef MAGAZIN_H
#define MAGAZIN_H

#include <QWidget>
#include <dbManager.h>
#include "mfile.h"
#include "magazinmodel.h"
#include "toollist.h"

namespace Ui {
class Magazin;
}

class Magazin : public QWidget
{
    Q_OBJECT

private:
    Ui::Magazin *ui;
    DBManager*     dbManager;
    MFile*         mfile;
    ToolList*      toolList;
    ToolList*      searchList;
    int            sizeIn;
    MagazinModel*  tableModel;
    QList<QString> list_ToolID;
    QList<QString> list_ToolDescription;
    QList<QString> list_ToolGL;
    QList<QString> list_ToolAL;
    QList<QString> list_ToolFL;
    QList<QString> list_ToolState;
    QList<QString> list_ToolSL;

    QString string_old;

    void contains(QString, ToolList*);
public:
    explicit Magazin(QWidget *parent = nullptr);
    ~Magazin();

    bool      contains(Tool*);
    bool      create_ToolList();
    int       get_Size(){return sizeIn;}
    ToolList* get_ToolList() {return toolList;}
    void      set_DBManager(DBManager* dbm) {dbManager = dbm;}
    void      set_FilePath(QString str){mfile->setFileName(str);}
    void      showToolList(ToolList*);

signals:
    void sig_Err(QString);
    void sig_Log(QString);

public slots:
    void slot_TableClicked(const QModelIndex &);
    void slot_textEdited(QString);
};

#endif // MAGAZIN_H
