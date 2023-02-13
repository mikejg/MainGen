#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

// Create a method to populate the model with data:
void TableModel::populateData(const QList<QString> &id,
                              const QList<QString> &description,
                              const QList<QString> &tgl,
                              const QList<QString> &tal,
                              const QList<QString> &tfl)
{
    list_ID.clear();
    list_ID = id;
    list_Description.clear();
    list_Description = description;
    list_TGL.clear();
    list_TGL = tgl;
    list_TAL.clear();
    list_TAL = tal;
    list_TFL.clear();
    list_TFL = tfl;

    return;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list_ID.length();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if (index.column() == 0)
    {
        return list_ID[index.row()];
    }
    else if (index.column() == 1)
    {
        return list_TGL[index.row()];
    }
    else if (index.column() == 2)
    {
        return list_TAL[index.row()];
    }
    else if (index.column() == 3)
    {
        return list_TFL[index.row()];
    }
    else if (index.column() == 4)
    {
        return list_Description[index.row()];
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (section == 0)
        {
            return QString("Tool ID");
        }
        else if (section == 1)
        {
            return QString("GL");
        }
        else if (section == 2)
        {
            return QString("AL");
        }
        else if (section == 3)
        {
            return QString("FL");
        }
        else if (section == 4)
        {
            return QString("Beschreibung");
        }
    }
    return QVariant();
}
