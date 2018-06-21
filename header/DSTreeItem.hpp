/**
  * @author Riccardo Damiani
  * @version 1.0 28/05/18
  */

#ifndef DSTREEITEM_H
#define DSTREEITEM_H

#include <QList>
#include <QVariant>


/**
 * @brief The DSTreeItem class
 * It is used to hold a list of QVariants, containing column data, and information about its
 * position in the tree structure.
 */
class DSTreeItem {
private:
    QList<DSTreeItem*> child_items;
    QList<QVariant> item_data;
    DSTreeItem* parent_item;
    bool checked;
public:
    DSTreeItem(const QList<QVariant>& data, DSTreeItem* parent = Q_NULLPTR);
    void appendChild(DSTreeItem* child);
    DSTreeItem* child(int row);
    int childCount() const;
    int row() const;
    int columnCount() const;
    QVariant data(int column) const;
    DSTreeItem* parentItem();
    bool isChecked() const;
    void setChecked(bool val);
    ~DSTreeItem();
};

#endif // DSTREEITEM_H
