#ifndef TP_BOHRUNG_H
#define TP_BOHRUNG_H

#include <QWidget>
#include "tp_base.h"

namespace Ui {
class TP_Bohrung;
}

class TP_Bohrung : public TP_Base
{
    Q_OBJECT

private:
    Ui::TP_Bohrung *ui;
    void eventFilter_Durchmesser(QEvent*);
    QStringList stringList_PostProcessing;
protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

public:
    explicit TP_Bohrung(QWidget *parent = nullptr);
    ~TP_Bohrung();
    void setPixmap();
    void set_Anfahren();
    QStringList postProcessing();
};

#endif // TP_BOHRUNG_H
