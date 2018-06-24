/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSTreeItem.hpp"


DSTreeItem::DSTreeItem(const QList<QVariant>& data, DSTreeItem* parent):
    item_data(data),
    parent_item(parent),
    checked(false)
{}

void DSTreeItem::appendChild(DSTreeItem* child) { child_items.append(child); }

DSTreeItem* DSTreeItem::child(int row) { return child_items.value(row); }

int DSTreeItem::childCount() const { return child_items.count(); }

int DSTreeItem::row() const {
    if(parent_item)
        return parent_item->child_items.indexOf(const_cast<DSTreeItem*>(this));

    return 0;
}

int DSTreeItem::columnCount() const { return item_data.count(); }

QVariant DSTreeItem::data(int column) const { return item_data.value(column); }

DSTreeItem *DSTreeItem::parentItem() { return parent_item; }

bool DSTreeItem::isChecked() const { return checked; }

void DSTreeItem::setChecked(bool val) { checked = val; }

DSTreeItem::~DSTreeItem() { qDeleteAll(child_items); }
