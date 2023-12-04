#ifndef TP_AUSRICHTEN_H
#define TP_AUSRICHTEN_H

#include <QWidget>
#include "tp_base.h"
#include <QComboBox>
#include <QStandardItemModel>

namespace Ui {
class TP_Ausrichten;
}

class TP_Ausrichten : public TP_Base
{
    Q_OBJECT

public:
    explicit TP_Ausrichten(QWidget *parent = nullptr);
    ~TP_Ausrichten();
    void set_Anfahren();
    QStringList postProcessing();
    void setPixmap();

private:
    Ui::TP_Ausrichten *ui;
    void SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled);
    QStringList stringList_PostProcessing;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

public slots:
    void slot_currentFrameChanged(QString);
    void slot_currentTextChanged(QString);

signals:
    //void sig_NewPixmap(QPixmap);
};

#endif // TP_AUSRICHTEN_H
