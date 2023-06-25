#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include "settings.h"

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

private:
    Ui::DialogSettings *ui;
    Settings* settings;
    QFileDialog* fileDialog;
    QPalette* paletteInValid;
    QPalette* paletteValid;
    bool send_SettingOK;

public:

    explicit DialogSettings(QWidget *parent = nullptr);
    ~DialogSettings();

    void set_Settings(Settings* s);
    //void set_ProgrammDir(QString s);
    //void set_VorlageSp1(QString s);
    //void set_VorlageSp2(QString s);
   //void set_VorlageSp3(QString s);
    //void set_Magazin(QString s);
    //void set_WerkzeugDB(QString s);
    //void set_Numbering(bool b);
    //void set_WerkzeugPlatze(int i);

    bool checkSettings();
signals:
    void sig_Genarete_MPF();
    void settingsOK();
    void sig_Log(QString);
    void sig_Err(QString);

public slots:
    void writeSettings();
    void toolButton_Programme_clicked(bool);
    /*
    void toolButton_VorlageSp1_clicked(bool);
    void toolButton_VorlageSp2_clicked(bool);
    void toolButton_VorlageSp3_clicked(bool);
    */
    void toolButton_Magazin_clicked(bool);
    void toolButton_WerkzeugDB_clicked(bool);
};

#endif // DIALOGSETTINGS_H
