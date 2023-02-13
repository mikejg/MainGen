#ifndef DIALOGRECOVER_H
#define DIALOGRECOVER_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTimer>
#include "toollist.h"
#include "parser.h"
#include "dbManager.h"

namespace Ui {
class DialogRecover;
}

class DialogRecover : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogRecover *ui;
    ToolList* toolList;
    QTimer* timer;
    QStringList stringList;
    Parser* parser;
    DBManager* dbManager;
    int counter;
    QStringList stringList_Split;
    QString string_Projekt;
    QString string_WiederholFertigung;

    void setValue(QString, int);
public:
    explicit DialogRecover(QWidget *parent = nullptr);
    ~DialogRecover();

    void reset_Progress();
    void setMaximum(int);
    void recover(QStringList);

    void setParser(Parser* p) {parser = p;}
    void setDBManager(DBManager* dbm) {dbManager = dbm;}

signals:
    void finished();
    void sig_Log(QString);
    void sig_Err(QString);

public slots:
    void slot_insertRPL();
};




#endif // DIALOGRECOVER_H
