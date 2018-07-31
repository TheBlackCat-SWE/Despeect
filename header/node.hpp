#ifndef NODE_H
#define NODE_H
#include "id.hpp"
#include <QGraphicsObject>

/**
 * @brief models a node, which is the graphic representation of an item
 */
 
class Node:public QGraphicsObject {
    Q_OBJECT
    
private:
    ID identifier;
    QString name;
    QColor color;
    QMap<std::string,std::string> features;
    const int radius;
public:
    
    /**
     * @brief Node constructor
     * @param id  id of the node
     * @param rel  name of the corresponding relation
     * @param path  node path
     * @param x x axis value
     * @param y y axis value
     * @param radius  node radius
     * @param color node color
     * @param parent  Qt graphic item as parent
     *
     */
    Node(const QString& id,const QString&rel,const QString& path, const int x, const int y, const int radius,
         const QColor& color, QGraphicsItem*parent, QMap<std::string,std::string> feat);
    
    /**
     * @brief redefinition of the == operator
     * @param other node to be compared
     * @return bool 
     */
    bool operator==(const Node& other)const;
    
    /**
     * @brief redefinition of the == operator
     * @param identifier  ID to be compared
     * @return bool 
     */
    bool operator==(const ID identifier);
    
    /**
     * @brief defines how to draw a node
     * @param painter  Qt painter
     * @param option  Qt style options for graphic items
     * @param widget  Qt widget as widget
     * @return void
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
    /**
     * @brief defines the bounding rectangle of the node so that the item position is at its center
     * @return QRectF
     */
    QRectF boundingRect()const;
    
    /**
     * @brief slot that is called when an item moves or changes, used to call the signal of position changing of a node
	 *				to notify all item observing the node about its movement
     * @param change  indicates the graphic item changing status
     * @param value  Qt variant as value
     * @return QVariant
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    
    /**
     * @brief slot that is called when an item is released from mouse click, used to call the signal of position changing of a node
     *				to notify all the items observing the node about its movement (done because of frame time which could not actually notify the last movement)
     * @param event Qt graphic scene mouse event as event
     * @return void
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
    /**
     * @brief returns the item ID
     * @return const QString &
     */
    const QString &getPath();
    
    /**
     * @brief returns the relation
     * @return const QString &
     */
    const QString &getRelation();

    /**
     * @brief returns the features
     * @return const QString &
     */

    QMap<std::string,std::string> getFeatures();
    
signals:

    /**
     * @brief signals to notify if visibility has changed to observing items
     * @param myVisibility visibility status
     * @return void
     */
    void notifyVisibilityChange(bool myVisibility);
    
    /**
     * @brief signals to notify if position has changed to observing items
     * @param const QPointF&  position status
     * @return void
     */
    void notifyPositionChange(const QPointF&);
    
public slots:
    
    /**
     * @brief slot to catch VisibilityChange of QGraphicsObject and call signal notifyVisibilityChange
     * @return void
     */
    void catchVisibilityChange();
};

#endif // NODE_H
