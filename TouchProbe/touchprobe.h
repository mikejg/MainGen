#ifndef TOUCHPROBE_H
#define TOUCHPROBE_H

#include <QWidget>
#include <QDir>
#include <mfile.h>
#include "project.h"

namespace Ui {
class TouchProbe;
}

class TouchProbe : public QWidget
{
    Q_OBJECT

private:
    Ui::TouchProbe *ui;
    MFile* mfile;
    QStringList stringList_Content;
    int         counter;
    void insert_Content();
    void export_Ebenheit();

    Project* project;

public:
    explicit TouchProbe(QWidget *parent = nullptr);
    ~TouchProbe();
    void insert_Item();
    void setProject(Project* p) { project = p;}

public slots:
    void slot_NewPixmap(QPixmap);
    void slot_Create();
    void slot_ShowPix();
    void slot_Export();

signals:
    void sig_Err(QString);
    void sig_Log(QString);
};

#endif // TOUCHPROBE_H
