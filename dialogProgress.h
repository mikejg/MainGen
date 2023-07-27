#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H
#include <QVBoxLayout>
#include <QDialog>
#include <QTimer>
#include <QDir>
#include <QProgressBar>
//#include "parser_programm.h"
//#include "dialogStart.h"

namespace Ui {
class DialogProgress;
}

class DialogProgress : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProgress(QWidget *parent = nullptr);
    ~DialogProgress();
    void setValue(QString, int);
    void setMaximum(int i);
    void initDialog(QStringList);

private:
    Ui::DialogProgress *ui;

public slots:

signals:
    void sig_Log(QString);
    void sig_Err(QString);
    void sig_Finished();
};

#endif // DIALOGWRITE_H
