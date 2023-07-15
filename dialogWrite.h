#ifndef DIALOGWRITE_H
#define DIALOGWRITE_H
#include <QVBoxLayout>
#include <QDialog>
#include <QTimer>
#include <QDir>
#include <QProgressBar>
#include "parser_programm.h"
#include "dialogStart.h"

namespace Ui {
class DialogWrite;
}

class DialogWrite : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWrite(QWidget *parent = nullptr);
    ~DialogWrite();
    void setValue(QString, int);

    void set_SRC(QString src) {string_SRC = src;}
    void set_DST(QString dst) {string_DST = dst;}
    void set_Programme(QStringList prg) {stringList_Programme = prg;}
    void set_Parser(Parser_Programm* p) {parser_Programm = p;}
    void set_ProgrammDir(QString str) {string_ProgrammDir =str;}
    void set_Projekt(QString str) {string_Projekt =str;}
    void set_Settings(DialogStart* ds) {dialogStart = ds;}
    void initDialog(QStringList);

private:
    Ui::DialogWrite *ui;
    QFile file;

    QString string_SRC;
    QString string_DST;
    QString string_Programm;
    QString string_ProgrammDir;
    QString string_Projekt;
    QString string_Log;

    QStringList stringList_Programme;


    bool bool_Numbering;

    Parser_Programm* parser_Programm;
    DialogStart* dialogStart;


    void finish_Antasten();
    void finish_Rohteil_Kontrolle();
    void finish_Reinigen();
public slots:

signals:
    void sig_Log(QString);
    void sig_Err(QString);
    void sig_Finished();
};

#endif // DIALOGWRITE_H
