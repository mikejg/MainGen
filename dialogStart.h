#ifndef DIALOGSTART_H
#define DIALOGSTART_H

#include "mfile.h"
#include "project.h"

#include <QCheckBox>
#include <QComboBox>

#include <QDialog>
#include <QDoubleSpinBox>
#include <QDir>

#include <QGroupBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QSettings>
#include <QVBoxLayout>

namespace Ui {
class DialogStart;
}

class DialogStart : public QDialog
{
    Q_OBJECT

private:
    Ui::DialogStart *ui;
    //bool firstStart = true;

    Project* project;
    MFile*   mfile_ZeroPoint;
    MFile*   mfile_Material;

    QPalette*  paletteInValid;
    QPalette*  paletteValid;
    QSettings* settings;

    QStringList stringList_ZeroPoint;

    QList<QString> list_Keys;

public:
    explicit DialogStart(QWidget *parent = nullptr);
    ~DialogStart();


    /*QLineEdit* lineEdit_ProjectName;
    QLineEdit* lineEdit_ProjectStatus;
    QLineEdit* lineEdit_RohteilX;
    QLineEdit* lineEdit_RohteilY;
    QLineEdit* lineEdit_RohteilZ;

    QLineEdit* lineEdit_BauteilX;
    QLineEdit* lineEdit_BauteilY;
    QLineEdit* lineEdit_BauteilZ;

    QDoubleSpinBox* doubleSpinBox_Aufmass_Xplus_Max;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Xminus_Max;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Yplus_Max;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Yminus_Max;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Zplus_Max;

    QDoubleSpinBox* doubleSpinBox_Aufmass_Xplus_Min;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Xminus_Min;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Yplus_Min;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Yminus_Min;
    QDoubleSpinBox* doubleSpinBox_Aufmass_Zplus_Min;

    QGroupBox* groupBox_AufmassMaxRT;

    QComboBox* comboBox_Material;
    QComboBox* comboBox_NP;
    QLineEdit* lineEdit_NP;

    QDoubleSpinBox* doubleSpinBox_ZRohTeil;*/

    QMap<QString, QString> map_NP;
    QString string_Line;

    //void setSettings(QSettings* s);
    void setProject(Project*);
    bool load_Material();
    bool load_ZeroPoint();

signals:
    void allValid();
    void sig_Log(QString);
    void sig_Err(QString);

public slots:
    //void showDialog();
    void slot_CheckInput();
    void slot_checkBox_RT_AufmassEinzel_stateChanged(int);
    void slot_checkBox_FT_AufmassEinzel_stateChanged(int);
    void slot_comboBox_NP_indexChanged(int);
};


#endif // DIALOGSTART_H
