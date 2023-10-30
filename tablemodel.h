#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QVariant>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent = 0);

    void populateData(const QList<QString> &id,
                      const QList<QString> &description,
                      const QList<QString> &tgl,
                      const QList<QString> &tal,
                      const QList<QString> &tfl,
                      const QList<QString> &tcounter);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<QString> list_ID;
    QList<QString> list_Description;
    QList<QString> list_TGL;
    QList<QString> list_TAL;
    QList<QString> list_TFL;
    QList<QString> list_Counter;

signals:

};



#endif // TABLEMODEL_H
