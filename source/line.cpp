#include "line.hpp"
#include <QVector2D>
#include <qpainter.h>

void Line::NodesColliding() {
    qreal x=MyPoints.x1()-MyPoints.x2();
    qreal y=MyPoints.y1()-MyPoints.y2();
    colliding=x*x+y*y<=(2*offsetFromPoint)*(2*offsetFromPoint);

}


Line::~Line() {
}

QPointF Line::FixStart() {
    //calculate direction vector and normalize it
    QVector2D directionVector(MyPoints.p1()-MyPoints.p2());
    directionVector.normalize();
    //return the fixed point of line
    return MyPoints.p1()-directionVector.toPointF()*offsetFromPoint;
}

QPointF Line::FixEnd() {
    //calculate direction vector and normalize it
    QVector2D directionVector(MyPoints.p1()-MyPoints.p2());
    directionVector.normalize();
    //return the fixed point of line
    return (MyPoints.p2()+directionVector.toPointF()*offsetFromPoint);
}


Line::Line(const int offset, const QColor&color, bool dashed, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,offsetFromPoint(offset)
    ,color(color)
    ,colliding(true)
    ,dashed(dashed) {
    this->color.setAlphaF(0.5);
}


void Line::UpdateStartPoint(const QPointF& startPoint) {
    //update start point to new point
    MyPoints.setP1(startPoint);
    //check if the two nodes it connects collide
    NodesColliding();
}

void Line::UpdateEndPoint(const QPointF &endPoint) {
    //update last point to new endpoint
    MyPoints.setP2(endPoint);
    //check if the two nodes it connect collide
    NodesColliding();
}

void Line::changeVisibility(bool vis) {
    //if has parent visibility depends on parent
    if(parentItem()==NULL){
        //vis is 1 if item changed to visible 0 otherwise
        //check if the line should be visible
        Visibility-=vis?-1:1;
        //if it should set it visible otherwise not visible
        if(Visibility==1)
            setVisible(true);
        else
            setVisible(false);
    }
}


QRectF Line::boundingRect() const {
    return QRectF(MyPoints.p1(),MyPoints.p2()).normalized();
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //if nodes are colliding don t draw
    if(colliding)return;
    //if line is dashed set the pen
    //to dashed
    QPen mypen(color);
    if(dashed)
    {   
        color.setAlphaF(1);
        mypen.setStyle(Qt::DashLine);
    }
    painter->setPen(mypen);
    //draw the line
    painter->drawLine(QLineF(FixStart(),FixEnd()));
}
