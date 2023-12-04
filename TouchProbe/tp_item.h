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
    enum State{NoState, Kante, Ausrichten, Steg, Bohrung, Ebenheit};
    void setText(QString str);
    QString getText();
    void read_Anfahren();
    QStringList postProcessing();
    QStringList postProcessing_Ebenheit();

    State getState() {return state;}
private:
    Ui::TP_Item *ui;
    bool bool_Fold;
    MFile*      mfile;
    QStringList stringList_ContentAnfahren;
    QStringList stringList_PostProcessing;
    QStringList stringList_PostProcessing_Ebenheit;
    void hideAll();

    State state;

public slots:
    void slot_Fold_In();
    void slot_Fold_Out();
    void slot_Fold_Toggle();
    void slot_Show_TP_Kante();
    void slot_Show_TP_Ausrichten();
    void slot_Show_TP_Steg();
    void slot_Show_TP_Ebenheit();
    void slot_Delet_Item();
    void slot_Show_TP_Bohrung();

signals:
    void sig_NewPixmap(QPixmap);
    void sig_NewItem();
    void sig_DeleteItem(TP_Item*);
    void sig_Err(QString);
    void sig_Log(QString);
};

#endif // TP_ITEM_H
