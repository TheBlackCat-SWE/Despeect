/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#ifndef DSTREEMODEL_H
#define DSTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class DSAdapter; // Avoiding circular dependencies technique
class DSTreeItem; // We only need the size of the obj here

/**
 * @brief The DSTreeModel class
 * This class interacts with the speect configuration through the adapter pointer
 * fetching the information needed by the QTreeView to display the data respecting
 * the model/view architecture.
 */
class DSTreeModel: public QAbstractItemModel {
    Q_OBJECT
private:
    DSAdapter* adapter;
    DSTreeItem* root;

    void setupModelData();
public:
    DSTreeModel(QObject* parent, DSAdapter* adapter);
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    ~DSTreeModel();
};

#endif // DSTREEMODEL_H
