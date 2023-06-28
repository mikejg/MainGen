#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QTableView>

#include "dbManager.h"
#include "dialogSettings.h"
#include "dialogStart.h"
#include "license.h"
#include "magazin.h"
#include "mfile.h"
#include "parser_placeholder.h"
#include "project.h"
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

    DBManager*          dbManager;
    DialogSettings*     dialogSettings;
    DialogStart*        dialogStart;
    License*            license;
    QList<QString>      list_ToolID;
    QList<QString>      list_ToolDescription;
    QList<QString>      list_ToolGL;
    QList<QString>      list_ToolAL;
    QList<QString>      list_ToolFL;
    Magazin*            magazin;
    MFile*              mfile;
    Parser_PlaceHolder* parser_PlaceHolder;
    Project*            project;
    Settings*           settings;
    TableModel*         tableModel;
    ToolList*           toolList_AllProjects;
    ToolList*           toolList_IN;
    ToolList*           toolList_OUT;
    ToolList*           toolList_Table;
    ToolList*           toolList_Top100;

    bool copyWerkzeugDB();
    void showTable_Rustplan();
    void showTable_Top100();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void frame_Error(QStringList);

signals:

public slots:
    void slot_dialogStart_Closed();
    void slot_Err(QString);
    void slot_LicenseFaild();
    void slot_Log(QString);
    void slot_Print(bool);
    void slot_PrintPage(QPrinter*);
    void slot_startApplication();

};
#endif // MAINWINDOW_H
