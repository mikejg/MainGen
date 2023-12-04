#ifndef TP_STEG_H
#define TP_STEG_H

#include <QWidget>
#include "tp_base.h"
namespace Ui {
class TP_Steg;
}

class TP_Steg : public TP_Base
{
    Q_OBJECT

private:
    Ui::TP_Steg *ui;
    void eventFilter_Wert(QEvent*);
    void eventFilter_DFA(QEvent*);
    void eventFilter_DZ(QEvent*);
    QStringList stringList_PostProcessing;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

public:
    explicit TP_Steg(QWidget *parent = nullptr);
    ~TP_Steg();
    void set_Anfahren();
    void setPixmap();
    QStringList postProcessing();

public slots:
    void slot_AxesChanged(QString);
};

#endif // TP_STEG_H
