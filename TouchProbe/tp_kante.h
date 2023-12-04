#ifndef TP_KANTE_H
#define TP_KANTE_H

#include <QWidget>
#include "tp_base.h"

namespace Ui {
class TP_Kante;
}

class TP_Kante : public TP_Base
{
    Q_OBJECT

private:
    Ui::TP_Kante *ui;
    void eventFilter_DFA(QEvent*);
    void eventFilter_Wert(QEvent*);
    QStringList stringList_PostProcessing;


protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

public:
    explicit TP_Kante(QWidget *parent = nullptr);
    ~TP_Kante();
    void set_Anfahren();
    QStringList postProcessing();
    void setPixmap();

public slots:
    void slot_AxesSignsChanged(QString);
    void slot_SignsChanged(QString);

signals:
    //void sig_NewPixmap(QPixmap);
};

#endif // TP_KANTE_H
