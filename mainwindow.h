#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableView>
#include <QTextEdit>
#include <QHeaderView>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>

#include "dialogSettings.h"
#include "dialogStart.h"
#include "dialogWrite.h"
#include "dialogrecover.h"
#include "dialogrepetition.h"
#include "toollist.h"
#include "parser.h"
#include "parser_wzm.h"
#include "parser_programm.h"
#include "tablemodel.h"
#include "tableprinter.h"
#include "dbManager.h"
#include "dbMainManager.h"
#include "license.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    DBManager* dbManager;
    dbMainManager* dbmainManager;

    DialogSettings* dialogSettings;
    DialogStart* dialogStart;
    DialogRecover* dialogRecover;
    DialogWrite* dialogWrite;
    DialogRepetition* dialogRepetition;

    QFileDialog* fileDialog;
    TableModel *tableModel;
    QTableView* tableView;
    QTableView* tableView_Top100;
    QTableView* tableView_Magazin;
    QTabWidget* tabWidget;
    QTextEdit* textEdit;
    QTextEdit* textEdit_Log;

    QAction *action_Open;
    QAction *action_Save;
    QAction *action_Print;
    QAction *action_FinishFile;
    QAction *action_ImportRpl;

    QString string_ProgrammDir;
    QString string_Projekt;
    QString string_ProjektName;
    QString string_ProjektStand;
    QString string_ProjektDir;
    QString string_VorlageSp1;
    QString string_VorlageSp2;
    QString string_MagazinDir;
    QString string_WerkzeugDB_orginal;
    QString string_Spannung;
    QString string_SpX;
    QString string_WiederholFertigung;


    QList<QString> list_ToolID;
    QList<QString> list_ToolDescription;
    QList<QString> list_ToolGL;
    QList<QString> list_ToolAL;
    QList<QString> list_ToolFL;

    QStringList stringList_Programme;

    QFile* file;
    QSettings* settings;
    QWidget* widget_Log;
    QWidget* widget_Ruestplan;

    ToolList* toolList_Top100;
    ToolList* toolList_Magazin;
    ToolList* toolList_Project;
    ToolList* toolList_IN;
    ToolList* toolList_OUT;
    ToolList* toolList_Table;
    ToolList* toolList_AllProjects;

    QList<ToolList*> list_SchwesterProjekte;


    Parser* parser;
    Parser_WZM* parser_Magazin;
    Parser_Programm* parser_Programm;
    License* license;

    enum Marker {KopfStart, KopfEnd, keinMarker};
    Marker marker;
    bool bool_Numbering;

    void erstelle_Magazin();

    void erstelle_ToolList();
    void finish_Rohteil_Kontrolle();
    bool load_Programme();
    bool openFile(QString fileName);
    void save_MPF(bool);
    void set_MainLayout();
    void set_Spannung(QString string_Vorlage);
    void showMagazin();
    void showTop100();
    void showTable();
    void writeG55();
    void disableAll();
    //void textEdit_append(QString);
    QString formatString(QString);
    void FileNameMax(QStringList);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void send_Value(QString, int);

public slots:
    void generate_MPF();
    void slot_Save(bool);
    void slot_CopyWerkzeugDB();
    void slot_CheckFiles(bool);
    void slot_Erstelle_Top100();
    void slot_Import_Ruestplan(bool);
    void slot_OpenRPL(bool);
    void slot_Print(bool);
    void slot_PrintPage(QPrinter* );
    void slot_RecoverDB(bool);
    void slot_SchwesterProjekt(bool);
    void slot_ShowSettings(bool);
    void slot_Log(QString);
    void slot_Err(QString);
    void slot_LicenseFaild();
    void slot_RepetitionAccepted();
    void slot_DifferentProjectFound();
    void slot_DialogStartRejected();
    void slot_DialogWriteFinished();
};
#endif // MAINWINDOW_H
