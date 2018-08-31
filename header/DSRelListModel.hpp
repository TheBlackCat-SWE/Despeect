#ifndef DSRELLISTMODEL_H
#define DSRELLISTMODEL_H

#include <QColor>

#include<QStandardItemModel>
#include <QStandardItem>

#include <QModelIndex>
#include <QVariant>


class DSAdapter;

class DSRelListModel: public QStandardItemModel {
    Q_OBJECT
private:
    DSAdapter* adapter;
    QStandardItem * parentItem;

signals:

public slots:
    //Using this slot should always be the only proper way to call setupModelData
    void fetchData();

public:
    DSRelListModel(QObject* parent, DSAdapter* adapter);

    QStringList getCheckedRelations() const;
    QStringList getAllRelations() const;
};


#endif // DSRELLISTMODEL_H
