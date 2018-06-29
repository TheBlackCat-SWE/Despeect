#ifndef DSRELLISTMODEL_H
#define DSRELLISTMODEL_H

#include <QColor>
#include <QStringListModel>
#include <QSet>
#include <QModelIndex>
#include <QVariant>


class DSAdapter;

class DSRelListModel: public QStringListModel {
    Q_OBJECT
private:
    DSAdapter* adapter;
    QSet<QPersistentModelIndex> checkedItems;

    //void setupModelData();

public slots:
    //Using this slot should always be the only proper way to call setupModelData
    void fetchData();
public:
    DSRelListModel(QObject* parent, DSAdapter* adapter);
    //int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index,const QVariant &value,int role) override;
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};


#endif // DSRELLISTMODEL_H
