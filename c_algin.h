#ifndef C_ALGIN_H
#define C_ALGIN_H

#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>

namespace Ui {
class C_Algin;
}

class C_Algin : public QWidget
{
    Q_OBJECT

private:
    Ui::C_Algin *ui;
    QStringList         stringList_Frames;
    QStringList         stringList_MessRichtung1;
    QStringList         stringList_MessRichtung2;

    void set_Oben();
    void SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled);

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

public:
    explicit C_Algin(QWidget *parent = nullptr);
    ~C_Algin();

public slots:
    void slot_currentFrameChanged(QString);
    void slot_currentTextChanged(QString);
};

#endif // C_ALGIN_H
