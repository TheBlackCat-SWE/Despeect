#include "DSRelListModel.hpp"
#include "DSAdapter.hpp"
#include <vector>
#include <string>
#include <QString>


void DSRelListModel::setupModelData() {
    std::vector<std::string> vec = adapter->getRelList();

    if(vec.empty()){
        list << "empty relation list";
    }
    else {

        for(auto&& str : vec)
            list << QString::fromStdString(str);
    }
}

void DSRelListModel::fetchData() {
    if(list.isEmpty()){
        list = QStringList();
    }
        setupModelData();
        layoutChanged();
}

DSRelListModel::DSRelListModel(QObject* parent, DSAdapter* adapter):
    QAbstractListModel(parent),
    adapter(adapter)
{}

int DSRelListModel::rowCount(const QModelIndex& parent) const {
    return list.size();
}

QVariant DSRelListModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(index.row() >= list.size())
        return QVariant();

    if(role == Qt::DisplayRole)
        return list.at(index.row());
    else
        return QVariant();
}

QVariant DSRelListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
        return QString("Key");
    else
        return QVariant();
}
