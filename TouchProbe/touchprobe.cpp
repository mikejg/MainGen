#include "touchprobe.h"
#include "ui_touchprobe.h"

TouchProbe::TouchProbe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TouchProbe)
{
    ui->setupUi(this);
    ui->textEdit->hide();
    ui->scrollAreaWidgetContents->set_Layout(ui->verticalLayout_scrollArea);
    ui->scrollAreaWidgetContents->set_SpacerItem(ui->verticalSpacer_ScrollContent);

    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));

    connect(ui->scrollAreaWidgetContents, SIGNAL(sig_Err(QString)),
            this, SIGNAL(sig_Err(QString)));
    connect(ui->scrollAreaWidgetContents, SIGNAL(sig_Log(QString)),
            this, SIGNAL(sig_Log(QString)));

    connect(ui->scrollAreaWidgetContents, SIGNAL(sig_NewPixmap(QPixmap)),
            this, SLOT(slot_NewPixmap(QPixmap)));

    connect(ui->toolButton_Edit, SIGNAL(pressed()), this, SLOT(slot_Create()));
    connect(ui->toolButton_Pix, SIGNAL(pressed()), this, SLOT(slot_ShowPix()));
    connect(ui->toolButton_Export, SIGNAL(pressed()),this, SLOT(slot_Export()));
}

TouchProbe::~TouchProbe()
{
    delete ui;
}

void TouchProbe::slot_Create()
{
    //Loesche alles im Editor und schalte auf den Editor um
    ui->textEdit->clear();
    ui->label_Pix->hide();
    ui->textEdit->show();

    //Zaehler auf 0 setzen
    counter = 0;

    //Kopf für das Antastpromm laden und in den Editor
    //mittels der Funktion insert_Content einfürgen
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Header.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    insert_Content();

    foreach(TP_Item* item, ui->scrollAreaWidgetContents->get_ItemList())
    {
        stringList_Content.clear();
        stringList_Content = item->postProcessing();
        insert_Content();
    }

    //Ende für das Antastpromm laden und in den Editor
    //mittels der Funktion insert_Content einfürgen
    mfile->setFileName(QDir::homePath() + "/MainGen/config/TouchProbe/Footer.txt");
    mfile->read_Content();
    stringList_Content = mfile->get_Content();
    insert_Content();
}

void TouchProbe::slot_ShowPix()
{
    ui->textEdit->hide();
    ui->label_Pix->show();
}

void TouchProbe::slot_NewPixmap(QPixmap p)
{
    ui->label_Pix->setPixmap(p);
}

void TouchProbe::insert_Item()
{
    ui->scrollAreaWidgetContents->slot_InsertItem();
}

void TouchProbe::insert_Content()
{
    foreach (QString str, stringList_Content)
    {
        if(str.startsWith("#"))
            continue;

        counter++;
        //Header
        str = str.replace("$G5x$",  project->get_ProjectZeroPoint());

        str = QString("N%1 ").arg(counter) + str;
        ui->textEdit->append(str);
    }
}

void TouchProbe::slot_Export()
{
    QString str;
    QDir dir;

    //Allei Einstellungen übernehmen und das Programm im Editor neu erstellen
    slot_Create();

    // Wenn es ProjecName oder ProjectFullName leer sind wird nur in das Programmverzeichnis geschrieben
    // Programme\_C_Ausrichten.SPF
    if(project->get_ProjectName().isEmpty() || project->get_ProjectFullName().isEmpty())
    {
        str = project->get_ProgrammDir() + "_Antasten.SPF";
    }

    // Ansonsten wird in die Datei
    // Programme\E12345678.WPD\E12345678_E01_Sp2.WPD\_C_Ausrichten.SPF
    // geschrieben
    else
    {
        str = project->get_ProgrammDir() + "/"
              + project->get_ProjectName() + ".WPD/"
              + project->get_ProjectFullName() + ".WPD/_Antasten.SPF";

        // Wenn es das Verzeichnis
        // Programme\E12345678.WPD\E12345678_E01_Sp2.WPD
        // noch nicht gibt dann erstelle es
        dir.setPath(project->get_ProgrammDir() + "/"
                    + project->get_ProjectName() + ".WPD/"
                    + project->get_ProjectFullName() + ".WPD");

        if(!dir.exists())
        {
            dir.mkpath(project->get_ProgrammDir() + "/"
                       + project->get_ProjectName() + ".WPD/"
                       + project->get_ProjectFullName() + ".WPD");
        }
    }

    QFile file(str);

    if(file.open(QFile::WriteOnly))
    {
        //qDebug() << "File is open";
        //Oeffne stream und schreib jede Zeile aus textEdit in den Stream
        QTextStream stream(&file);
        foreach(QString string_Line, ui->textEdit->toPlainText().split("\n"))
        {
            //qDebug() << string_Line;
            stream << string_Line << "\n";
        }
        file.close();
    }

    export_Ebenheit();
}

void TouchProbe::export_Ebenheit()
{
    qDebug() << Q_FUNC_INFO;

    QString str;
    QDir dir;

    foreach(TP_Item* item, ui->scrollAreaWidgetContents->get_ItemList())
    {

        if(item->getState() == TP_Item::Ebenheit)
        {
            counter = 0;
            stringList_Content.clear();
            foreach (QString str, item->postProcessing_Ebenheit())
            {
                if(str.startsWith("#"))
                    continue;

                counter++;
                //Header
                str = str.replace("$G5x$",  project->get_ProjectZeroPoint());

                str = QString("N%1 ").arg(counter) + str;
                stringList_Content.append(str);
            }
            //stringList_Content = item->postProcessing_Ebenheit();
            //insert_Content();

            // Wenn es ProjecName oder ProjectFullName leer sind wird nur in das Programmverzeichnis geschrieben
            // Programme\_C_Ausrichten.SPF
            if(project->get_ProjectName().isEmpty() || project->get_ProjectFullName().isEmpty())
            {
                str = project->get_ProgrammDir() + "_Ebenheit.SPF";
            }

            // Ansonsten wird in die Datei
            // Programme\E12345678.WPD\E12345678_E01_Sp2.WPD\_C_Ausrichten.SPF
            // geschrieben
            else
            {
                str = project->get_ProgrammDir() + "/"
                      + project->get_ProjectName() + ".WPD/"
                      + project->get_ProjectFullName() + ".WPD/_Ebenheit.SPF";

                // Wenn es das Verzeichnis
                // Programme\E12345678.WPD\E12345678_E01_Sp2.WPD
                // noch nicht gibt dann erstelle es
                dir.setPath(project->get_ProgrammDir() + "/"
                            + project->get_ProjectName() + ".WPD/"
                            + project->get_ProjectFullName() + ".WPD");

                if(!dir.exists())
                {
                    dir.mkpath(project->get_ProgrammDir() + "/"
                               + project->get_ProjectName() + ".WPD/"
                               + project->get_ProjectFullName() + ".WPD");
                }
            }

            QFile file(str);

            if(file.open(QFile::WriteOnly))
            {
                //qDebug() << "File is open";
                //Oeffne stream und schreib jede Zeile aus textEdit in den Stream
                QTextStream stream(&file);
                foreach(QString string_Line, stringList_Content)
                {
                    //qDebug() << string_Line;
                    stream << string_Line << "\n";
                }
                file.close();
            }
        }
    }
}
