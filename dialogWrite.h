#ifndef DIALOGWRITE_H
#define DIALOGWRITE_H
#include <QVBoxLayout>
#include <QDialog>
#include <QTimer>
#include <QDir>
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

    void set_SRC(QString src) {string_SRC = src;}
    void set_DST(QString dst) {string_DST = dst;}
    void set_Programme(QStringList prg) {stringList_Programme = prg;}
    void set_Parser(Parser_Programm* p) {parser_Programm = p;}
    void set_ProgrammDir(QString str) {string_ProgrammDir =str;}
    void set_Projekt(QString str) {string_Projekt =str;}
    void set_Settings(DialogStart* ds) {dialogStart = ds;}
    void initDialog(bool);
private:
    Ui::DialogWrite *ui;

    QFile file;
    QTimer* timer;
    QString string_SRC;
    QString string_DST;
    QString string_Programm;
    QString string_ProgrammDir;
    QString string_Projekt;
    QString string_Log;

    QStringList stringList_Programme;

    int counter;
    bool bool_Numbering;

    Parser_Programm* parser_Programm;
    DialogStart* dialogStart;

    void setValue(QString, int);
    void finish_Antasten();
    void finish_Rohteil_Kontrolle();
    void finish_Reinigen();
public slots:
    void slot_nextProgramm();

signals:
    void sig_Log(QString);
    void sig_Err(QString);
};

#endif // DIALOGWRITE_H
