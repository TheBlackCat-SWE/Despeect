/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSListModel.hpp"
#include "DSAdapter.hpp"
#include <vector>
#include <string>
#include <QString>


void DSListModel::setupModelData() {
    std::vector<std::string> vec = adapter->getFeatProcList();

    for(auto&& str : vec)
        list << QString::fromStdString(str);
}

DSListModel::DSListModel(QObject* parent, DSAdapter* adapter):
    QAbstractListModel(parent),
    adapter(adapter)
{
    setupModelData();
}

int DSListModel::rowCount(const QModelIndex& parent) const {
    return list.size();
}

QVariant DSListModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(index.row() >= list.size())
        return QVariant();

    if(role == Qt::DisplayRole)
        return list.at(index.row());
    else
        return QVariant();
}

QVariant DSListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
        return QString("Key");
    else
        return QVariant();
}
