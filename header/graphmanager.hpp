#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QStandardItemModel>
#include <QListView>
#include "datanodemanager.hpp"
#include "id.hpp"
#include "node.hpp"
class DSItem;
class Node;

/**
 * @brief manage graphs and relations to keep the two models coherent
 */
 
class GraphManager : public QObject {
    Q_OBJECT
    
private:
    static const int Radius=15;
    //map of all the relations and the items that represent the relations on the graph
    QMap<QString,QGraphicsRectItem*> Relations;
    
public:
    //model of the relation printed on the graph
    QStandardItemModel* RelationsModel;
    //list of all the nodes printed on the graph
    QMap<QString, Node*> Printed;
    QGraphicsScene* Graph;
    GraphManager();
    ~GraphManager();
    GraphManager(const GraphManager&)=delete;

    /**
     * @brief returns the graph model to be linked with the view
     * @param  view  Qt graphic view as view component
     * @return void
     */
    void linkGraphModel(QGraphicsView* view);

    /**
     * @brief returns the relation model to be linked with the view
     * @param view Qt list view
     * @return void
     */
    void linkRelationModel(QListView*);

    /**
     * @brief prints the relations which start from the given item, with the chosen name and the chosen color
     * @param id Qt string as relation name
     * @param SpeectNode initial item
     * @param Color Qt color as chosen color
     * @return bool
     */
    bool printRelation(const QString& id, const DSItem* SpeectNode, const QColor& Color);
    void clear();
    
private:
    /**
     * @brief generates the relation item to use as parent of all nodes and arcs
     * @param id Qt string as relation name
     * @param Color Qt color as relation color
     * @return QGraphicsRectItem *
     */
    QGraphicsRectItem *generateRelation(const QString& id,const QColor& color);
    
    /**
     * @brief generates the item to be added into the relations model
     * @param const QString& - Qt string as relation name (see Qt docs for more info) 
     * @param const QColor & - Qt color as relation color (see Qt docs for more info)
     * @return QStandardItem *
     */
    QStandardItem *generateItem(const QString& id, const QColor & color);
    
    /**
     * @brief positions the node in the first column where it doesn't collide
     * @param me node reference
     * @return void
     */
    void PositionNode(Node& me);
    
    /**
     * @brief positions the node in the first row where it doesn't collide
     * @param me node reference
     * @return void
     */
    void FixHeadPosition(Node& me);
    
    /**
     * @brief check the relation of the first item in the list, the node that graphically represents the item
     * 				must be in Printed QVector, this method check the relations of the item and correctly
     *				add to the list of nodes that must be checked the next and the daughter of the item
     * @param tbc items vector
     * @param relation Qt string as relation name
     * @param color Qt color as relation color
     * @param parentRelation Qt graphics item as parent relation (see Qt docs for more info)
     * @return void
     */
    void checkRelations(QVector<const DSItem*>& tbc, const QString &relation, const QColor &color, QGraphicsItem *parentRelation);

public slots:

    void changeRelationVisibilityList(QStringList,QStringList);

    /**
     * @brief changes the visibility of the relations in the graph
     * @param key Qt standard item as relation reference
     * @return void
     */
    void changeRelationVisibility(QStandardItem * key);
    
    /**
     * @brief notifies the selection/deselection of a item
     * @return void
     */
    void notifySelection();
    
    /**
     * @brief focuses the node given the relation and the path
     * @param relation Qt string as relation name
     * @param path Qt string as path
     * @return void
     */
    void selectItem(const QString& relation,const QString& path);
    
signals:
    
    /**
     * @brief signal launched when there is only one item focused and it's a node
     * @param const QString& - Qt string as relation name (see Qt docs for more info) 
     * @param const QString& - Qt string as relation ID (see Qt docs for more info) 
     * @param bool - focus/unfocus
     * @return void
     */
	void focusSignal(const QString&, const QString&, bool);
	void cleardetails();
};

#endif // GRAPHMANAGER_H
