#include "modelheaderrequestdata.h"

#include <QDebug>

ModelHeaderRequestData::ModelHeaderRequestData(QObject *parent) :
    QAbstractTableModel(parent),
    m_rawsHeader(QList<RawHeader>())
{

}

QList<RawHeader> ModelHeaderRequestData::rawsHeader()
{
    return m_rawsHeader;
}

void ModelHeaderRequestData::addNewRawHeader(RawHeader data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_rawsHeader.append(data);
    endInsertRows();
}

void ModelHeaderRequestData::removeOneRawHeader(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_rawsHeader.removeAt(index);
    endRemoveRows();
}

int ModelHeaderRequestData::columnCount(const QModelIndex &) const
{
    return static_cast<int>(Column_COUNT);
}

int ModelHeaderRequestData::rowCount(const QModelIndex &) const
{
    return m_rawsHeader.count();
}

QVariant ModelHeaderRequestData::data(const QModelIndex &index, int role) const
{
    if((index.row() >= 0) || (index.row() < rowCount()) || (index.column() >= 0) || (index.column() < static_cast<int>(Column_COUNT)))
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == static_cast<int>(Column_Name))
            {
                return m_rawsHeader[index.row()].name;
            }
            else if(index.column() == static_cast<int>(Column_Value))
            {
                return m_rawsHeader[index.row()].value;
            }
        }
    }

    //qDebug() << __PRETTY_FUNCTION__ << "error index=" << index;
    return QVariant();
}
