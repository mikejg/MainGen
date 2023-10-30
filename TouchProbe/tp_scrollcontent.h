#ifndef TP_SCROLLCONTENT_H
#define TP_SCROLLCONTENT_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "tp_item.h"


QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

namespace Ui
{
class TP_ScrollContent;
}

class TP_ScrollContent : public QWidget
{
    Q_OBJECT

private:
    Ui::TP_ScrollContent *ui;
    QList<TP_Item*> item_List;
    TP_Item* tp_Item;
    QVBoxLayout* layout;
    QSpacerItem* spacerItem;
    TP_Item *child;

public:
    explicit TP_ScrollContent(QWidget *parent = nullptr);
    ~TP_ScrollContent();
    void set_Layout(QVBoxLayout* l) {layout = l;}
    void set_SpacerItem(QSpacerItem* si) {spacerItem = si;}
    void insert_Item(QString);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void sig_NewPixmap(QPixmap);
};

#endif // TP_SCROLLCONTENT_H
