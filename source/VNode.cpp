// myitem.cpp

#include "VNode.hpp"

int VNode::length = 30;

VNode::VNode(int x, int y, DSItem *info, QColor color)
    :QGraphicsObject(nullptr),
     x(x),
     y(y),
     info(info),
     color(color)
{
  //  setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRectF VNode::boundingRect() const
{
    // outer most edges
    return QRectF(x-5,y-5,length+10,length+25);
}

void VNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    QPen pen(Qt::black, 3);
    painter->setPen(pen);
    painter->setBrush(color);
    painter->drawEllipse(x,y,length,length);

    rect.setTop(rect.bottom()-20);
    rect.setBottom(rect.bottom());
    rect.setLeft(rect.left()-20);
    rect.setRight(rect.right()+20);
    painter->drawText(rect,Qt::AlignHCenter|Qt::TextDontClip, info->getName().c_str());
}

bool VNode::operator ==(const VNode& other) const {
    return *info == *(other.info);
}
