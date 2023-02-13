#ifndef HOLDERLIST_H
#define HOLDERLIST_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "holder.h"
class HolderList : public QObject
{
    Q_OBJECT

private:
    QList<Holder*> list;

public:
    explicit HolderList(QObject *parent = nullptr);
    void insert_Holder(Holder*);
signals:

};

#endif // HOLDERLIST_H
