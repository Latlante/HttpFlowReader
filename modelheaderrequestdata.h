#ifndef MODELHEADERREQUESTDATA_H
#define MODELHEADERREQUESTDATA_H

#include <QAbstractTableModel>

#include "common.h"

class ModelHeaderRequestData : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum DataByColumn
    {
        Column_Name = 0,
        Column_Value,
        Column_COUNT
    };

    explicit ModelHeaderRequestData(QObject *parent = nullptr);

    QList<RawHeader> rawsHeader();

    void addNewRawHeader(RawHeader data);
    void removeOneRawHeader(int index);

    virtual int columnCount(const QModelIndex & = QModelIndex()) const;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

signals:

private:
    QList<RawHeader> m_rawsHeader;
};

#endif // MODELHEADERREQUESTDATA_H
