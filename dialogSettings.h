#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QVBoxLayout>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

private:
    Ui::DialogSettings *ui;
    QSettings* settings;
    QFileDialog* fileDialog;
    QPalette* paletteInValid;
    QPalette* paletteValid;
    bool send_SettingOK;

public:

    explicit DialogSettings(QWidget *parent = nullptr);
    ~DialogSettings();

    void set_Settings(QSettings* s) {settings = s;}
    void set_ProgrammDir(QString s);
    void set_VorlageSp1(QString s);
    void set_VorlageSp2(QString s);
    void set_VorlageSp3(QString s);
    void set_Magazin(QString s);
    void set_WerkzeugDB(QString s);
    void set_Numbering(bool b);

    bool checkSettings();
signals:
    void sig_Genarete_MPF();
    void settingsOK();

public slots:
    void writeSettings();
    void toolButton_Programme_clicked(bool);
    void toolButton_VorlageSp1_clicked(bool);
    void toolButton_VorlageSp2_clicked(bool);
    void toolButton_VorlageSp3_clicked(bool);
    void toolButton_Magazin_clicked(bool);
    void toolButton_WerkzeugDB_clicked(bool);
};

#endif // DIALOGSETTINGS_H
