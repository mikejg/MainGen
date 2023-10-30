#include "tp_scrollcontent.h"
#include "ui_tp_scrollcontent.h"

#include <QtWidgets>

TP_ScrollContent::TP_ScrollContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TP_ScrollContent)
{
    ui->setupUi(this);
}

TP_ScrollContent::~TP_ScrollContent()
{
    delete ui;
}

void TP_ScrollContent::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug() << "Start: " << Q_FUNC_INFO;

    if (event->mimeData()->hasFormat("application/x-dnditemdata"))
    {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else
    {
        event->ignore();
    }
    //qDebug() << "Ende: " << Q_FUNC_INFO;

}

void TP_ScrollContent::dragMoveEvent(QDragMoveEvent *event)
{
    //qDebug() << "Start: " << Q_FUNC_INFO;

    if (event->mimeData()->hasFormat("application/x-dnditemdata"))
    {
        //qDebug() << "if (event->mimeData()->hasFormat(\"application/x-dnditemdata\"))";
        if (event->source() == this)
        {
            //qDebug() << "if (event->source() == this)";
            event->setDropAction(Qt::MoveAction);
            event->accept();


        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
    //qDebug() << "Ende: " << Q_FUNC_INFO;

}
void TP_ScrollContent::dropEvent(QDropEvent *event)
{
    //qDebug() << "Start: " << Q_FUNC_INFO;
    if (event->mimeData()->hasFormat("application/x-dnditemdata"))
    {
        //qDebug() << "application/x-dnditemdata";
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString string;
        QPoint offset;
        dataStream >> string >> offset;
        //qDebug() << "STRING: " << string;

        child->move(event->pos() - offset);
        if (event->source() == this)
        {
            //qDebug() << "if (event->source() == this)";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }

        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        //qDebug() << "event->ignonre";
        event->ignore();
    }

    //qDebug() << "End: " << Q_FUNC_INFO;
}

//! [1]
void TP_ScrollContent::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "Start: " << Q_FUNC_INFO;
    child = static_cast<TP_Item*>(childAt(event->pos()));

    if(child == nullptr) return;

    while(child->parent() != this)
    {
        child = static_cast<TP_Item*>(child->parent());
    }

    //qDebug() << "Start:01 " << Q_FUNC_INFO;
    if (!child)
        return;

    //qDebug() << "Start:02 " << Q_FUNC_INFO;
    //child->setInValid();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QString string = child->getText();
    dataStream << string << QPoint(event->pos() - child->pos());
//! [1]

//! [2]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
//! [2]

//! [3]
    QPixmap pixmap = child->grab();
    QPainter painter;
    painter.begin(&pixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());
//! [3

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    {
        //qDebug() << "(drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)";
        //child->close();
    }
    else
    {
        //qDebug() << "else (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) ";
        child->show();
    }

    /*Alle Widgets aus dem Layout loeschen*/
    layout->removeItem(spacerItem);
    foreach(TP_Item* item, item_List)
    {
        this->layout->removeWidget(item);
    }

   for (int i = 0; i<item_List.size(); i++)
   {

       for(int j = 0; j<i; j++)
       {
           if(item_List.at(i)->geometry().y() < item_List.at(j)->geometry().y())// + item_List.at(j)->geometry().height())
           {
               item_List.move(i,j);
               break;
           }
       }
   }

   //qDebug() << "nach dem Sortieren";

   foreach(TP_Item* item, item_List)
   {
       //qDebug() << item->getText() << ": " <<item->geometry();
       this->layout->addWidget(item);
   }
   layout->addItem(spacerItem);
   //qDebug() << "Ende: " << Q_FUNC_INFO;
}


void TP_ScrollContent::insert_Item(QString str)
{
    layout->removeItem(spacerItem);
    tp_Item = new TP_Item(this);
    tp_Item->setText(str);
    layout->addWidget(tp_Item);
    item_List.append(tp_Item);
    layout->addItem(spacerItem);
    connect(tp_Item, SIGNAL(sig_NewPixmap(QPixmap)), this, SIGNAL(sig_NewPixmap(QPixmap)));
}
