#ifndef DBMAINMANAGER_H
#define DBMAINMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include "tool.h"

class dbMainManager : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase m_db;

public:
    explicit dbMainManager(QObject *parent = nullptr);
    void open();
    void insertTool(QString, Tool*);
    QString insertRuestplan(QString);
signals:

};

#endif // DBMAINMANAGER_H
