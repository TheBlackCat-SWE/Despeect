#ifndef LINE_H
#define LINE_H
#include <QGraphicsObject>

/**
 * @brief graphic line connecting two nodes
 */

class Line:public QGraphicsObject
{
    Q_OBJECT
private:
    int Visibility=1;
protected:
    //radius of the node to offset from its center
    const int offsetFromPoint;
    QColor color;
    //the start and ending point of the line
    QLineF MyPoints;
    bool colliding;
    bool dashed;
    /**
     * @brief checks if the nodes are colliding
     * @return void
     */
    virtual void NodesColliding();
    /**
     * @brief fix the position of start and end to be the closest of the circle of the two nodes,
     * 				used when drawing not to draw from center to center of the nodes
     */
    virtual QPointF FixStart();
    virtual QPointF FixEnd();

public:

    /**
     * @brief Line constructor
     * @param radius radius to offset
     * @param color Qt color as line color
     * @param dashed indicates if the line is dashed
     * @param parent Qt graphics item as parent
     */
    Line(const int radius, const QColor& color, bool dashed=0, QGraphicsItem*parent=0);
    virtual ~Line();

public slots:

    /**
     * @brief slots to update the position of the line, used to update position when the item is
     * 				connected to move the QPointF given at the center of the object
     * @param startPoint Qt point as starting point
     * @return void
     */
    void UpdateStartPoint(const QPointF& startPoint);
    void UpdateEndPoint(const QPointF& endPoint);

    /**
     * @brief set visibility to:
     * 				- 1  if the line must be drawn
     * 				- 0  if one node is hidden
     * 				- -1 if both
     * @param vis  indicates the visibility state
     * @return void
     */
    void changeVisibility(bool vis);

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};
#endif // LINE_H
