#include "DSRelListModel.hpp"
#include "DSAdapter.hpp"
#include <vector>
#include <string>
#include <QString>


//void DSRelListModel::setupModelData() {

//}


void DSRelListModel::fetchData() {
    std::vector<std::string> vec = adapter->getRelList();

    QStringList res = QStringList();
    if(vec.empty()) {
        //res << "Empty relations list";
    }
    else {
        for(std::vector<std::string>::iterator it = vec.begin();
            it != vec.end();
            ++it) {
            res << QString::fromStdString(*it);
        }
    }
    setStringList(res);
}

DSRelListModel::DSRelListModel(QObject* parent, DSAdapter* adapter):
    QStringListModel(parent),
    adapter(adapter)
{}


QVariant DSRelListModel::data(const QModelIndex& index, int role) const {

    if (!index.isValid())
        return QVariant();

    if(role == Qt::CheckStateRole)
        return checkedItems.contains(index) ?
                    Qt::Checked : Qt::Unchecked;

    else if(role == Qt::BackgroundColorRole)
        return checkedItems.contains(index) ?
                    QColor("#ffffb2") : QColor("#ffffff");

    return QStringListModel::data(index, role);
}


bool DSRelListModel::setData(const QModelIndex& index,const QVariant &value,int role) {

    if(!index.isValid() || role != Qt::CheckStateRole)
        return false;

    if(value == Qt::Checked)
        checkedItems.insert(index);
    else
        checkedItems.remove(index);

    emit dataChanged(index, index);
    return true;
}


Qt::ItemFlags DSRelListModel::flags(const QModelIndex& index) const {

    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
    if (index.isValid()){
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}

QStringList DSRelListModel::getRelationsAsQStringList() const {
    QStringList keys = QStringList();
    foreach(QPersistentModelIndex index, checkedItems) {
        keys << index.data().toString();
    }
    return keys;
}
