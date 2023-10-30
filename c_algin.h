#ifndef C_ALGIN_H
#define C_ALGIN_H

#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>

#include "mfile.h"
#include "project.h"

namespace Ui {
class C_Algin;
}

class C_Algin : public QWidget
{
    Q_OBJECT

private:
    Ui::C_Algin *ui;

    MFile*      mfile;
    Project*    project;
    QStringList stringList_Frames;
    QStringList stringList_MessRichtung1;
    QStringList stringList_MessRichtung2;
    QStringList stringList_Content;
    QStringList stringList_ContentAnfahren;
    int         int_CAxis;
    int         int_AAxis;
    int         counter;
    QString     string_CYCLE998;

    void insert_Content();
    QString replace_Comma(QString);

    //void set_Oben();
    void SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled);

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

public:
    explicit C_Algin(QWidget *parent = nullptr);
    ~C_Algin();

    void setProject(Project* p) {project = p;}

signals:
    void sig_Err(QString);
    void sig_Log(QString);

public slots:
    void slot_currentFrameChanged(QString);
    void slot_currentTextChanged(QString);
    void slot_create();
    void slot_export();
    void slot_pix();
};

#endif // C_ALGIN_H
