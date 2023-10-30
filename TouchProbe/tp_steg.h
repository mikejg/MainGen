#ifndef TP_STEG_H
#define TP_STEG_H

#include <QWidget>

namespace Ui {
class TP_Steg;
}

class TP_Steg : public QWidget
{
    Q_OBJECT

public:
    explicit TP_Steg(QWidget *parent = nullptr);
    ~TP_Steg();

private:
    Ui::TP_Steg *ui;
};

#endif // TP_STEG_H
