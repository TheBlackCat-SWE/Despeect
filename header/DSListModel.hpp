/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#ifndef DSLISTMODEL_H
#define DSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QModelIndex>
#include <QVariant>


class DSAdapter;

class DSListModel: public QAbstractListModel {
    Q_OBJECT
private:
    DSAdapter* adapter;
    QStringList list;

    void setupModelData();
public:
    DSListModel(QObject* parent, DSAdapter* adapter);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // DSLISTMODEL_H
