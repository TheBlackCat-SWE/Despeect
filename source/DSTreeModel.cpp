/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSTreeModel.hpp"
#include "DSTreeItem.hpp"
#include "DSAdapter.hpp"
#include <map>
#include <string>
#include <QList>
#include <QString>


void DSTreeModel::setupModelData() {
    std::map<std::string, std::vector<std::string>> dict =
            adapter->getUttTypeDict();

    QList<DSTreeItem*> parents; // This allows me to access to the last parent

    for(auto&& dict_pair : dict) {
        QList<QVariant> utt_type_data;
        utt_type_data << QString::fromStdString(dict_pair.first) << QString("Utt Type");
        parents << new DSTreeItem(utt_type_data, root);
        root->appendChild(parents.last());
        for(auto&& utt_proc : dict_pair.second) {
            QList<QVariant> utt_proc_data;
            utt_proc_data << QString::fromStdString(utt_proc) << QString("Utt Proc");
            parents.last()->appendChild(new DSTreeItem(utt_proc_data, parents.last()));
        }
    }
}

DSTreeModel::DSTreeModel(QObject* parent, DSAdapter* adapter):
    QAbstractItemModel(parent),
    adapter(adapter)
{
    QList<QVariant> root_data;
    root_data << "Key" << "Type";
    root = new DSTreeItem(root_data);
    setupModelData();
}

QModelIndex DSTreeModel::index(int row, int column, const QModelIndex& parent) const {
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    DSTreeItem* parent_item;

    if(!parent.isValid())
        parent_item = root;
    else
        parent_item = static_cast<DSTreeItem*>(parent.internalPointer());

    DSTreeItem* child_item = parent_item->child(row);
    if(child_item)
        return createIndex(row, column, child_item);
    else
        return QModelIndex();
}

QModelIndex DSTreeModel::parent(const QModelIndex& index) const {
    if(!index.isValid())
        return QModelIndex();

    DSTreeItem* child_item = static_cast<DSTreeItem*>(index.internalPointer());
    DSTreeItem* parent_item = child_item->parentItem();

    if(parent_item == root)
        return QModelIndex();

    return createIndex(parent_item->row(), 0, parent_item);
}

int DSTreeModel::rowCount(const QModelIndex& parent) const {
    DSTreeItem *parent_item;
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        parent_item = root;
    else
        parent_item = static_cast<DSTreeItem*>(parent.internalPointer());

    return parent_item->childCount();
}

int DSTreeModel::columnCount(const QModelIndex& parent) const {
    if(parent.isValid())
        return static_cast<DSTreeItem*>(parent.internalPointer())->columnCount();
    else
        return root->columnCount();
}

QVariant DSTreeModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    DSTreeItem* item = static_cast<DSTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags DSTreeModel::flags(const QModelIndex &index) const {
    if(!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DSTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root->data(section);

    return QVariant();
}

DSTreeModel::~DSTreeModel() { delete root; }
