#ifndef ARC_H
#define ARC_H
#include "line.hpp"
#include <QGraphicsObject>

/**
 * @brief draws a line with an arrow on its end point
 */
class Arc:public Line {
    Q_OBJECT
    
private:
    const int arrowWidth;
    const bool isfilled;
    const bool type;
    QPointF arrowPoint1;
    QPointF arrowPoint2;
    
    /**
     * @brief fix the end point and arrow points when nodes move
     * @return QPointF
     */
    QPointF FixEnd();

public:

    /**
     * @brief Arc constructor
     * @param arrowWidth
     * @param radius radius of the pointed nodes, it is needed to offset from the center
     * @param color Qt color as the arc color
     * @param prev_next indicates the kind of arc
     * @param filled indicates if the arrow head is filled
     * @param parent  Qt graphic item
     * @param dashed indicates if the arc is dashed
     */
    Arc(const int arrowWidth, const int radius, const QColor& color, bool prev_next=1, bool filled=1, QGraphicsItem*parent=0, bool dashed=0);

public:
    
    /**
     * @brief redefines the paint function to draw the arrow of the arc
     * @param painter  Qt painter
     * @param option  Qt style options for the graphic item
     * @param widget Qt widget
     * @return void
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:

};
#endif // ARC_H
