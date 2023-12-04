#ifndef TP_EBENHEIT_H
#define TP_EBENHEIT_H

#include <QWidget>
#include "tp_base.h"

namespace Ui {
class TP_Ebenheit;
}

class TP_Ebenheit : public TP_Base
{
    Q_OBJECT
private:
    Ui::TP_Ebenheit *ui;
    QStringList stringList_PostProcessing;
    QStringList stringList_Ebenheit;

public:
    explicit TP_Ebenheit(QWidget *parent = nullptr);
    ~TP_Ebenheit();

    QStringList postProcessing();
    QStringList postProcessing_Ebenheit();
    void setPixmap();
};

#endif // TP_EBENHEIT_H
