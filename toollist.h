#ifndef TOOLLIST_H
#define TOOLLIST_H

#include "tool.h"
#include <QObject>
#include <QList>

class ToolList : public QObject
{
    Q_OBJECT

private:
    QList<Tool*> list;

public:
    explicit ToolList(QObject *parent = nullptr);
    void insert_Tool(Tool*, bool check=true);
    QList<Tool*> getList() {return list;};
    void sort_Counter();
    //void sort_Top100();
    void sort_ID();
    void clear() {list.clear();};
    bool contains(Tool*);
    void set_Description(ToolList* tl);
    void prepend(Tool*);
    int getSize() {return list.size();};
signals:

};

#endif // TOOLLIST_H
