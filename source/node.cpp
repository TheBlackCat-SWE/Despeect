#include "node.hpp"
#include <QFocusEvent>
#include <QPainter>
#include <QGraphicsScene>


Node::Node(const SItem* item,const QString& id,const QString& rel,const QString& path, const int x, const int y, const int radius,
           const QColor& color, QGraphicsItem*parent, QMap<std::string,std::string> feat)
   :QGraphicsObject(parent)
   ,core(item)
   ,color(color)
   ,identifier(id,rel)
   ,name(path)
   ,radius(radius)
   ,features(feat){
    //link QGraphicsObject signal to the slot
    connect(this,SIGNAL(visibleChanged()),this,SLOT(catchVisibilityChange()));
    //allow item movement selection and allow item notification to scene Model
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    //enable itemChange slot
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    //set the item position
    setX(x);
    setY(y);
    //so that is over arrow but not over lines
    setZValue(1);
}


bool Node::operator==(const Node &other) const {
    return identifier==other.identifier;
}


bool Node::operator==(const ID identifier) {
    return identifier==this->identifier;
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    //default pen and brush
    QPen myPen = QPen(Qt::black);

    color.setAlphaF(0.5);
    QBrush Brush=QBrush(color);
    //when selected
    if(isSelected())
    {

        QColor tempC=color;
        tempC.setAlphaF(1);
        myPen.setWidth(5);
        Brush.setColor(tempC);
        myPen.setStyle(Qt::SolidLine);
    }//only when focused
    else if(hasFocus())
    {
        QColor tempC=color;
        tempC.setAlphaF(1);
        myPen.setColor(tempC);
        Brush.setColor(color);
        myPen.setWidth(5);

    }

    //set the brush and color
    painter->setBrush(Brush);
    painter->setPen(myPen);
    //draw and ellipse inside the bounding rect of this item
    QRectF t=boundingRect();
    //bounding rect is a square so ellipse is a circle
    painter->drawEllipse(t);
    //draw the name info at bottom of item
    t.setBottom(t.top());
    t.setTop(t.top()-15);
    painter->drawText(t,Qt::AlignCenter|Qt::TextDontClip,name);

}

QRectF Node::boundingRect() const {
    //draw the rect so that it's position is in the middle of rect
    return QRectF(-radius,-radius,radius*2,radius*2);
}


QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    //if changed position notify all my arcs my movement
    if (change == ItemPositionChange&&scene()) {
        notifyPositionChange(pos());
    }
    //if selected for search reason set own flag
    return QGraphicsItem::itemChange(change,value);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    //call super
    QGraphicsItem::mouseReleaseEvent(event);
    //notify all arcs my last position
    notifyPositionChange(pos());
}


const QString &Node::getPath() {
    return identifier.getPath();
}


const QString &Node::getRelation() {
    return identifier.getRelation();
}


void Node::catchVisibilityChange() {
    notifyVisibilityChange(isVisible());
}

QMap<std::string,std::string> Node::getFeatures(){
    return features;
}

const SItem* Node::getSItem()const{
    return core;
}

