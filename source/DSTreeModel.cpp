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
#include <QModelIndex>


void DSTreeModel::setupModelData() {
    if(root) delete root;
    QList<QVariant> root_data;
    root_data << "Key" << "Type";
    root = new DSTreeItem(root_data);

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

void DSTreeModel::fetchData() {
    setupModelData();
    layoutChanged();
}

DSTreeModel::DSTreeModel(QObject* parent, DSAdapter* adapter):
    QAbstractItemModel(parent),
    adapter(adapter)
{
    QList<QVariant> root_data;
    root_data << "Key" << "Type";
    root = new DSTreeItem(root_data);
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
    if(index.isValid()) {
        DSTreeItem* item = static_cast<DSTreeItem*>(index.internalPointer());

        if(role == Qt::DisplayRole)
            return item->data(index.column());

        if(role == Qt::CheckStateRole && index.column() == 0)
            return static_cast<int>(item->isChecked() ? Qt::Checked : Qt::Unchecked);
    }
    return QVariant();
}

Qt::ItemFlags DSTreeModel::flags(const QModelIndex& index) const {
    if(!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() == 0)
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

bool DSTreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if(index.isValid() && role == Qt::CheckStateRole && index.column() == 0) {
        DSTreeItem* item = static_cast<DSTreeItem*>(index.internalPointer());
        item->setChecked(value.toBool());

        //Tick-untick all the childs' checkboxes
        if(item->childCount() != 0)
            for(int i = 0; i < item->childCount(); i++)
                setData(index.child(i, index.column()), value, role);

        //Untick all the utt_procs' checkboxes belonging to other utt_types
        if(static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked) {
            QModelIndex current_utt_type = index;
            //Climb the tree until you reach the utt_type node
            do {
                current_utt_type = current_utt_type.parent();
            }
            while(parent(current_utt_type).isValid());
            //Loop over all the utt_types unticking them cascade style unless it hits
            //the current one
            QModelIndex i = current_utt_type.sibling(0,0);
            while(i.isValid()) {
                if(i != current_utt_type)
                    setData(i, Qt::Unchecked, role);
                i = i.sibling(i.row() + 1,0);
            }
        }

        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

QVariant DSTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root->data(section);

    return QVariant();
}

DSTreeModel::~DSTreeModel() { delete root; }
