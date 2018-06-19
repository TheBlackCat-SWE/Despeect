#ifndef VNODE_H
#define VNODE_H

#include "DSItem.hpp"
#include <QPainter>
#include <QGraphicsItem>

// class for customization
class VNode :public QGraphicsObject
{
    Q_OBJECT
private:
    int x;
    int y;
    QColor color;

public:
    static int length;
    static int distance;

    DSItem* info;
    VNode(int x, int y, DSItem* info, QColor color);
    QRectF boundingRect() const;
    QRectF textRect() const;

    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
    bool operator ==(const VNode&) const;

    QPointF getCenter() const {
        return QPointF(x+length/2, y+length/2);
    }
};

#endif // VNODE_H
