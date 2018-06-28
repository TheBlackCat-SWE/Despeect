#ifndef DSRELLISTMODEL_H
#define DSRELLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QModelIndex>
#include <QVariant>


class DSAdapter;

class DSRelListModel: public QAbstractListModel {
    Q_OBJECT
private:
    DSAdapter* adapter;
    QStringList list;

    void setupModelData();
public slots:
    //Using this slot should always be the only proper way to call setupModelData
    void fetchData();
public:
    DSRelListModel(QObject* parent, DSAdapter* adapter);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};


#endif // DSRELLISTMODEL_H
