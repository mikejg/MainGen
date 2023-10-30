#ifndef DIALOGROHTEILKONTROLLE_H
#define DIALOGROHTEILKONTROLLE_H

#include <QDialog>
#include "project.h"

namespace Ui {
class DialogRohteilkontrolle;
}

class DialogRohteilkontrolle : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogRohteilkontrolle *ui;
    Project* project;

public:
    explicit DialogRohteilkontrolle(QWidget *parent = nullptr);
    ~DialogRohteilkontrolle();

    void setProject(Project* p) {project = p;}

signals:
    void sig_New_Rohteilkontrolle(QString);
public slots:
    void slot_Selected();
};

#endif // DIALOGROHTEILKONTROLLE_H
