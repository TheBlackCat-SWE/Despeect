#include "DSRelListModel.hpp"
#include "DSAdapter.hpp"
#include <vector>
#include <string>
#include <QString>

#include<QStandardItem>


void DSRelListModel::fetchData() {

    clear();

    if(adapter->nullUtterance() || adapter->hasError()){
        return;
    }

    std::vector<std::string> vec = adapter->getRelList();

    parentItem = invisibleRootItem();

    for(unsigned long i = 0; i < vec.size();++i){
        QStandardItem *item = new QStandardItem();
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->setData(QString::fromStdString(vec[i]),Qt::DisplayRole);
        parentItem->appendRow(item);
    }

}

QStringList DSRelListModel::getAllRelations() const{
    QStringList allRelations = QStringList();

    for(int i = 0 ; i < parentItem->rowCount();++i){
        allRelations << parentItem->child(i,0)->text();
    }
    return allRelations;
}

QStringList DSRelListModel::getCheckedRelations() const{

    QStringList checkedRelations = QStringList();

    for(int i = 0 ; i < parentItem->rowCount();++i){
        if(parentItem->child(i,0)->checkState() == Qt::Checked){
            checkedRelations << parentItem->child(i,0)->text();
        }
    }

    return checkedRelations;
}

DSRelListModel::DSRelListModel(QObject* parent, DSAdapter* adapter):
    QStandardItemModel(parent),
    adapter(adapter)
{
}




