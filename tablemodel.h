#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QBrush>
#include <QColor>
#include <QFont>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent = 0);

    void populateData(const QList<QString> &contactName,
                      const QList<QString> &contactPhone,
                      const QList<QString> &tgl,
                      const QList<QString> &tal,
                      const QList<QString> &tfl);

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

signals:

};



#endif // TABLEMODEL_H
