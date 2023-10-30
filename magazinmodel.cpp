#include "magazinmodel.h"

MagazinModel::MagazinModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

// Create a method to populate the model with data:
void MagazinModel::populateData(const QList<QString> &id,
                              const QList<QString> &description,
                              const QList<QString> &tgl,
                              const QList<QString> &tal,
                              const QList<QString> &tfl,
                              const QList<QString> &status,
                              const QList<QString> &tsl  )
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
    list_Status.clear();
    list_Status = status;
    list_TSL.clear();
    list_TSL = tsl;
    return;
}

int MagazinModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list_ID.length();
}

int MagazinModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant MagazinModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch (role)
    {
    case Qt::DisplayRole:
        if(col == 0) return QString::fromUtf8( "\u2B24" );
        if(col == 1) return list_ID[row];
        if(col == 2) return list_TGL[row];
        if(col == 3) return list_TAL[row];
        if(col == 4) return list_TFL[row];
        if(col == 5) return list_TSL[row];
        if(col == 6) return list_Description[row];

    case Qt::TextAlignmentRole:
        if(col == 0) return Qt::AlignCenter;

    case Qt::ForegroundRole:
        if(col == 0 && list_Status[row] == "In") return QBrush(Qt::darkGreen);
        if(col == 0 && list_Status[row] == "Out") return QBrush(Qt::darkYellow);
        if(col == 0 && list_Status[row] == "Disassembled") return QBrush(Qt::darkRed);
    }
    return QVariant();
}

QVariant MagazinModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (section == 0)
        {
            return QString::fromUtf8( "\u2B24" );//QString("Tool ID");
        }
        else if (section == 1)
        {
            return QString("Tool ID");
        }
        else if (section == 2)
        {
            return QString("GL");
        }
        else if (section == 3)
        {
            return QString("AL");
        }
        else if (section == 4)
        {
            return QString("FL");
        }
        else if (section == 5)
        {
            return QString("SL");
        }
        else if (section == 6)
        {
            return QString("Beschreibung");
        }
    }
    return QVariant();
}
