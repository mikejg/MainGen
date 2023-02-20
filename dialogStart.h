#ifndef DIALOGSTART_H
#define DIALOGSTART_H

#include "parser_programm.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QSettings>
#include <QDir>
#include <QComboBox>
#include <QDoubleSpinBox>

namespace Ui {
class DialogStart;
}

class DialogStart : public QDialog
{
    Q_OBJECT

private:
    Ui::DialogStart *ui;
    bool firstStart = true;
    QString string_SPx;

    Parser_Programm* parser_Programm;

    QPalette* paletteInValid;
    QPalette* paletteValid;
    QSettings* settings;

    void readMaterial();

public:
    explicit DialogStart(QWidget *parent = nullptr);
    ~DialogStart();

    QLineEdit* lineEdit_Projekt;
    QLineEdit* lineEdit_ProjektStand;
    QLineEdit* lineEdit_RohteilX;
    QLineEdit* lineEdit_RohteilY;
    QLineEdit* lineEdit_RohteilZ;

    QRadioButton* radioButton_Sp1;
    QRadioButton* radioButton_Sp2;

    //QCheckBox* checkBox_WriteG55;
    QStringList stringList_Material;
    QComboBox* comboBox_Material;

    QDoubleSpinBox* doubleSpinBox_ZRohTeil;

    void loadProjectName();
    void setSettings(QSettings* s) {settings = s;}
    void setParser_Programm(Parser_Programm* p) {parser_Programm = p;}

signals:
    void allValid();
    void sig_DifferentProjectFound();
    void sig_Log(QString);
    void sig_Err(QString);

public slots:
    void showDialog();
    void slot_CheckInput();
    void slot_Sp1_Toggled(bool);
};


#endif // DIALOGSTART_H
