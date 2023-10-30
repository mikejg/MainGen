#ifndef TP_BASE_H
#define TP_BASE_H

#include <QWidget>
#include <QDir>
#include "mfile.h"

//namespace Ui {
//class TP_Base;
//}

class TP_Base : public QWidget
{
    Q_OBJECT

private:
    //Ui::TP_Base *ui;
    MFile*      mfile;


public:
    explicit TP_Base(QWidget *parent = nullptr);
    ~TP_Base();
    QStringList stringList_ContentAnfahren;

signals:
    void sig_Err(QString);
    void sig_Log(QString);
};

#endif // TP_BASE_H
