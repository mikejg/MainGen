#include "magazin.h"
#include "ui_magazin.h"

Magazin::Magazin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Magazin)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mfile = new MFile(this);
    connect(mfile, SIGNAL(sig_Log(QString)), this, SIGNAL(sig_Log(QString)));
    connect(mfile, SIGNAL(sig_Err(QString)), this, SIGNAL(sig_Err(QString)));

    connect(ui->lineEdit, SIGNAL(textEdited(QString)), this, SLOT(slot_textEdited(QString)));
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slot_TableClicked(const QModelIndex &)));

    toolList = new ToolList(this);
    searchList = new ToolList(this);
    sizeIn = 0;

    QPalette palette;
    palette.setColor(QPalette::Highlight, ui->tableView->palette().color(QPalette::Base));
    palette.setColor(QPalette::HighlightedText, ui->tableView->palette().color(QPalette::Text));

    ui->tableView->setPalette(palette);
}

Magazin::~Magazin()
{
    delete ui;
}

bool Magazin::contains(Tool* tool)
{
    foreach (Tool* toolList_Tool, toolList->getList())
    {
        if(tool->get_Number() == toolList_Tool->get_Number()
            && toolList_Tool->get_State() == 0)
        {
            return true;
        }

        if(toolList_Tool->get_Number().contains(tool->get_Number()) &&
            toolList_Tool->get_State() == Tool::Disassembled)
            tool->set_State(Tool::Disassembled);
    }

    return false;
}

void Magazin::contains(QString str, ToolList* list)
{
    searchList->clear();
    foreach (Tool* tool, toolList->getList())
    {
        if(tool->get_Number().startsWith(str))
            list->insert_Tool(tool, false);
    }
}

bool Magazin::create_ToolList()
{
    QStringList stringList_ToolData;

    if(!mfile->read_Content())
        return false;
    toolList->clear();
    QString string_ToolID;
    QString string_Temp;
    bool bool_X;
    Tool* tool = new Tool(this);
    foreach(QString string_Line, mfile->get_Content())
    {
        bool_X = false;
        /* Wenn string_Line mit $TC_TP2 startet
         * schneide solage von Links ein Zeichen ab bis du auf ein " stoesst
         * entferne am ende das "
         * schreibe den uebrig gebliebenen string_Line in string_ToolID
         * */
        if(string_Line.startsWith("$TC_TP2"))
        {
            while(!string_Line.startsWith("\"") && string_Line.size() > 0)
            {
                string_Line = string_Line.right(string_Line.length()-1);
            }
            string_Line.remove("\"");
            string_ToolID=string_Line;
        }

        /* Wenn string_Line mit $A_TOOLMN startet
         * schneide solage von Links ein Zeichen ab bis du auf ein = stoesst
         * entferne am Ende das =
         * Wenn string_Line nicht leer ist und string_Line nicht 0 ist
         * erstelle das Tool und schreibe ist in die toolList */

        if(string_Line.startsWith(";$A_TOOLMN"))
        {
            while(!string_Line.startsWith("=") && string_Line.size() > 0)
            {
                string_Line = string_Line.right(string_Line.length()-1);
            }
            string_Line.remove("=");

            if(!string_Line.isEmpty() && string_Line != "0")
            {
                tool->set_State(Tool::In);
                sizeIn++;
            }
            else
            {
                tool->set_State(Tool::Out);
            }

            if(string_ToolID.contains("X") || string_ToolID.contains("x"))
            {
                tool->set_State(Tool::Disassembled);
                bool_X = true;
                string_ToolID = string_ToolID.remove("_X");
                string_ToolID = string_ToolID.remove("_x");
            }
            if(string_ToolID.contains("_"))
            {
                stringList_ToolData = dbManager->getToolData(string_ToolID);
                if(stringList_ToolData.size() < 5)
                {
                    sig_Err("Unvollstandige Tooldaten: " + string_ToolID);
                    continue;
                }
                tool->clear();
                tool->set_Number(string_ToolID);
                tool->set_Description(stringList_ToolData.at(3));
                //tool->set_Description(dbManager->getDescription(string_ToolID));

                string_Temp = stringList_ToolData.at(1);
                if(string_Temp.length() > 7)
                    string_Temp = string_Temp.left(7);
                tool->set_ToolAL(string_Temp);

                string_Temp = stringList_ToolData.at(2);
                if(string_Temp.length() > 7)
                    string_Temp = string_Temp.left(7);
                tool->set_ToolFL(string_Temp);

                string_Temp = stringList_ToolData.at(0);
                if(string_Temp.length() > 7)
                    string_Temp = string_Temp.left(7);
                tool->set_ToolGL(string_Temp);

                string_Temp = stringList_ToolData.at(4);
                if(string_Temp.length() > 7)
                    string_Temp = string_Temp.left(7);
                tool->set_SchneidLange(string_Temp);

                if(bool_X)
                    string_ToolID += "_X";
                tool->set_Number(string_ToolID);

                toolList->insert_Tool(tool, false);
            }
        }
    }

    foreach (tool, toolList->getList())
    {
        tool->set_counter(dbManager->getCounter(tool->get_Number()));
    }

    /*
    foreach (tool, toolList->getList())
    {
        qDebug() << tool->get_Number() << ": " << tool->get_counter();
    }
    */
    showToolList(toolList);
    return true;
}

void Magazin::showToolList(ToolList* list)
{
    list->sort_ID();
    list_ToolID.clear();
    list_ToolDescription.clear();
    list_ToolGL.clear();
    list_ToolAL.clear();
    list_ToolFL.clear();
    list_ToolState.clear();
    list_ToolSL.clear();

    //Füge die Werkzeuge ein
    foreach(Tool* tool, list->getList())
    {
        list_ToolDescription.append(tool->get_Description());
        list_ToolID.append(tool->get_Number());
        list_ToolGL.append(tool->get_ToolGL());
        list_ToolAL.append(tool->get_ToolAL());
        list_ToolFL.append(tool->get_ToolFL());
        list_ToolState.append(tool->get_StateString());
        list_ToolSL.append(tool->get_SchneidLange());
    }

    tableModel = new MagazinModel(this);

    // Populate model with data:
    tableModel->populateData(list_ToolID,
                             list_ToolDescription,
                             list_ToolGL,
                             list_ToolAL,
                             list_ToolFL,
                             list_ToolState,
                             list_ToolSL);

    ui->tableView->setModel(tableModel);

}

void Magazin::slot_TableClicked(const QModelIndex &modelIndex)
{
    QPalette palette;

    if(list_ToolState[modelIndex.row()] == "In")
    {
        palette.setColor(QPalette::Highlight, ui->tableView->palette().color(QPalette::Base));
        palette.setColor(QPalette::HighlightedText, Qt::green);
    }
    else if(list_ToolState[modelIndex.row()] == "Out")
    {
        palette.setColor(QPalette::Highlight, ui->tableView->palette().color(QPalette::Base));
        palette.setColor(QPalette::HighlightedText, Qt::yellow);
    }
    else if(list_ToolState[modelIndex.row()] == "Disassembled")
    {
        palette.setColor(QPalette::Highlight, ui->tableView->palette().color(QPalette::Base));
        palette.setColor(QPalette::HighlightedText, Qt::red);
    }
    else
    {
        palette.setColor(QPalette::Highlight, ui->tableView->palette().color(QPalette::Base));
        palette.setColor(QPalette::HighlightedText, ui->tableView->palette().color(QPalette::Text));
    }
    ui->tableView->setPalette(palette);
}

void Magazin::slot_textEdited(QString str)
{
    //qDebug() << "str: " << str.length();
    QString string;
    if(str.length() < string_old.length())
    {
        string_old = ui->lineEdit->text();
        contains(string_old, searchList);
        showToolList(searchList);
        return;
    }

    if(str.length() > 2 && !str.contains("_"))
    {
        string = ui->lineEdit->text();
        string = string.insert(2,"_");
        ui->lineEdit->setText(string);
    }

    if(str.length() > 6 && str.at(6) != QString("_"))
    {
        string = ui->lineEdit->text();
        string = string.insert(6,"_");
        ui->lineEdit->setText(string);
    }
    string_old = ui->lineEdit->text();

    contains(string_old, searchList);
    showToolList(searchList);
}
