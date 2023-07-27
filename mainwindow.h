#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QTableView>

#include "c_algin.h"
#include "dbManager.h"
#include "dialogrepetition.h"
#include "dialogSettings.h"
#include "dialogStart.h"
#include "license.h"
#include "magazin.h"
#include "mfile.h"
#include "parser_placeholder.h"
#include "project.h"
#include "project_loader.h"
#include "project_saver.h"
#include "tablemodel.h"
#include "tableprinter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    QPalette* paletteValid;
    QPalette* paletteInValid;

    QAction *action_AddFile;
    QAction *action_Export;
    QAction *action_FinishFile;
    QAction *action_ImportRpl;
    QAction *action_Open;
    QAction *action_Print;
    QAction *action_RestoreDatabase;
    QAction *action_ShowSettings;

    C_Algin*            c_Algin;
    DBManager*          dbManager;
    DialogRepetition*   dialogRepetition;
    DialogSettings*     dialogSettings;
    DialogStart*        dialogStart;
    DialogProgress*     dialogProgress;
    License*            license;
    QList<QString>      list_ToolID;
    QList<QString>      list_ToolDescription;
    QList<QString>      list_ToolGL;
    QList<QString>      list_ToolAL;
    QList<QString>      list_ToolFL;
    Magazin*            magazin;
    MFile*              mfile;
    Parser_PlaceHolder* parser_PlaceHolder;
    Parser_Programm*    parser_Programm;
    Project_Loader*     project_Loader;
    Project_Saver*      project_Saver;
    Project*            project;
    Settings*           settings;
    QStringList         stringList_Programme;
    TableModel*         tableModel;
    ToolList*           toolList_AllProjects;
    ToolList*           toolList_IN;
    ToolList*           toolList_OUT;
    ToolList*           toolList_Table;
    ToolList*           toolList_Top100;

    bool copyWerkzeugDB();
    bool load_Programme();
    void FileNameMax(QStringList);
    void showTable_Rustplan();
    void showTable_Top100();


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void frame_Error(QStringList);

signals:

public slots:

    void slot_AddFile(bool);
    void slot_dialogStart_Closed();
    void slot_Err(QString);
    void slot_Export(bool b);
    void slot_FinishFile(bool b);
    void slot_LicenseFaild();
    void slot_Log(QString);
    void slot_Open(bool);
    void slot_Print(bool);
    void slot_PrintPage(QPrinter*);
    void slot_RepetitionAccepted();
    void slot_RestoreDatabase(bool);
    void slot_ShowSettings(bool);
    void slot_startApplication();


};
#endif // MAINWINDOW_H
