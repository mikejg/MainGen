#ifndef TP_ITEM_H
#define TP_ITEM_H

#include <QFrame>
#include <QPixmap>
#include "mfile.h"

namespace Ui {
class TP_Item;
}

class TP_Item : public QFrame
{
    Q_OBJECT

public:
    explicit TP_Item(QWidget *parent = nullptr);
    ~TP_Item();
    void setText(QString str);
    QString getText();

private:
    Ui::TP_Item *ui;
    bool bool_Fold;
    MFile*      mfile;
    QStringList stringList_ContentAnfahren;

public slots:
    void slot_Fold_In();
    void slot_Fold_Out();
    void slot_Fold_Toggle();
    void slot_Show_TP_Kante();

signals:
    void sig_NewPixmap(QPixmap);
};

#endif // TP_ITEM_H
