#ifndef DIALOGREPETITION_H
#define DIALOGREPETITION_H

#include <QDialog>
#include <QRadioButton>

namespace Ui {
class DialogRepetition;
}

class DialogRepetition : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRepetition(QWidget *parent = nullptr);
    ~DialogRepetition();

    QRadioButton* radioButton_Repetition;
    QRadioButton* radioButton_Paket;
private:
    Ui::DialogRepetition *ui;
};

#endif // DIALOGREPETITION_H
