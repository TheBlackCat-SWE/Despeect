#include "graphmanager.hpp"
#include <QFocusEvent>
#include <QGraphicsItemGroup>
#include <QGraphicsView>
#include "arc.hpp"
#include "DSItem.hpp"
#include "node.hpp"

/*
 * File: graphmanager.cpp
 * Type: src
 * Date: 2018-04-23
 * E-mail: graphite.swe@gmail.com
 * 
 * Description: manage graphs and relations to keep the two models coherent 
 */

// Description: GraphManager constructor 
GraphManager::GraphManager()
    :Graph(new QGraphicsScene())
    ,Relations()
    ,RelationsModel(new QStandardItemModel())
{
    //when an item in the relations model get checked or unchecked graph manager will tell to the graph to hide or show the relation
    connect(RelationsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(changeRelationVisibility(QStandardItem*)));
    connect(Graph,SIGNAL(selectionChanged()),this,SLOT(notifySelection()));
}

// Description: GraphManager destructor
GraphManager::~GraphManager()
{
    clear();
    delete Graph;
    delete RelationsModel;
}

/*
 * Description: returns the graph model to be linked with the view
 * @param QGraphicsView* view - Qt graphic view as view component (see Qt docs for more info)
 * @return void
 */
void GraphManager::linkGraphModel(QGraphicsView* v)
{
    v->setScene(Graph);
}

/*
 * Description: returns the relation model to be linked with the view
 * @param QListView* - Qt list view (see Qt docs for more info)
 * @return void
 */
void GraphManager::linkRelationModel(QListView* v)
{
    v->setModel(RelationsModel);
}

/*
 * Description: prints the relations which start from the given item, with the chosen name and the chosen color
 * @param const QString& - Qt string as relation name (see Qt docs for more info)
 * @param const Item* - initial item
 * @param const QColor& - Qt color as chosen color (see Qt docs for more info)
 * @return bool
 */
bool GraphManager::printRelation(const QString &id, const DSItem *SpeectNode, const QColor &Color)
{
    //items that must have their relations check
    QVector<const DSItem*>ToBeChecked;
    //generate the relation
    QGraphicsRectItem* parentRelation=generateRelation(id,Color);
    //if no error in generation of relations
    if(parentRelation!=NULL){
    //add to graph the relation
        Graph->addItem(parentRelation);
        //add to nodes that must be checked the head node
        ToBeChecked.push_back(SpeectNode);
        Node* t=new Node(QString::fromStdString(SpeectNode->getPath()),id,QString::fromStdString(SpeectNode->getName()),Radius+10,Radius+20,Radius,Color,parentRelation);
        Printed.insert(QString::fromStdString(SpeectNode->getId()),t);
        FixHeadPosition(*t);
        //cicle till there is no more nodes to be checked
        while(!ToBeChecked.isEmpty())
        {
            //fix the relations and print next and daughter
            checkRelations(ToBeChecked,id,Color,parentRelation);
        }
    }
}

/*
 * Description: clears both models
 * @return void
 */
#include "iostream"
void GraphManager::clear()
{
    //clear the vector and the Relations
    Relations.clear();
    Printed.clear();
    //clear the graph (delete all items inside)
    Graph->clear();

    //clear the relationsModel and delete all item in it
    for(int i=0;i<RelationsModel->rowCount();++i)
    {
        delete RelationsModel->item(i);
    }
    RelationsModel->clear();
}


/*
 * Description: generates the relation item to use as parent of all nodes and arcs 
 * @param const QString& - Qt string as relation name (see Qt docs for more info) 
 * @param const QColor& - Qt color as relation color (see Qt docs for more info)
 * @return QGraphicsRectItem *
 */
QGraphicsRectItem *GraphManager::generateRelation(const QString &id, const QColor &color)
{
    if(!Relations.contains(id)) {
        RelationsModel->appendRow(generateItem(id,color));
        QGraphicsRectItem* t=new QGraphicsRectItem();
        Relations.insert(id,t);
        return t;
     }
    return NULL;

}

/*
 * Description: generates the item to be added into the relations model
 * @param const QString& - Qt string as relation name (see Qt docs for more info) 
 * @param const QColor & - Qt color as relation color (see Qt docs for more info)
 * @return QStandardItem *
 */
QStandardItem *GraphManager::generateItem(const QString &id,const QColor&color)
{
    QStandardItem* item=new QStandardItem(id);
    item->setBackground(QBrush(color));
    item->setCheckable(true);
    item->setEditable(false);
    item->setCheckState(Qt::Checked);
    item->setSelectable(false);
    return item;
}

/*
 * Description: positions the node in the first column where it doesn't collide
 * @param Node& - node reference
 * @return void
 */
void GraphManager::PositionNode(Node &me)
{
    while(me.collidingItems().size()>1){
        me.setX(me.x()+(4*Radius));
    }
}

/*
 * Description: positions the node in the first row where it doesn't collide
 * @param Node& - node reference
 * @return void
 */
void GraphManager::FixHeadPosition(Node &me)
{
    while(me.collidingItems().size()>1){
        me.setY(me.y()+(4*Radius));
    }
}

/*
 * Description: check the relation of the first item in the list, the node that graphically represents the item
 * 				must be in Printed QVector, this method check the relations of the item and correctly
 *				add to the list of nodes that must be checked the next and the daughter of the item
 * @param QVector<const Item*>& - items vector
 * @param const QString &relation - Qt string as relation name (see Qt docs for more info) 
 * @param const QColor &color - Qt color as relation color (see Qt docs for more info)
 * @param QGraphicsItem *parentRelation - Qt graphics item as parent relation (see Qt docs for more info)
 * @return void
 */
void GraphManager::checkRelations(QVector<const DSItem*> &tbc, const QString& relation, const QColor& color, QGraphicsItem *parentRelation)
{
    //take first item
    const DSItem* toBeChecked=tbc.takeFirst();

    //get the items
    const DSItem* parent=toBeChecked->parent();
    const DSItem* child=toBeChecked->child();
    const DSItem* next=toBeChecked->next();
    const DSItem* previous=toBeChecked->previous();

    //find which node represent the items
    Node* me = Printed.value(QString::fromStdString(toBeChecked->getId()));

    //if find the node continue otherwise stop and fail
    if(me)
    {
        foreach(Node* node,Printed)
        {
            //If item is equal and have different relation draw a dashed line
            if(toBeChecked->getRelation() != node->getRelation().toStdString() && toBeChecked->getPath() == node->getPath().toStdString())
            {
                Line* a=new Line(Radius,QColor(qRgb(0,0,0)),1,NULL);
                connect(node,SIGNAL(notifyVisibilityChange(bool)),a,SLOT(changeVisibility(bool)));
                connect(node,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateEndPoint(QPointF)));
                connect(me,SIGNAL(notifyVisibilityChange(bool)),a,SLOT(changeVisibility(bool)));
                connect(me,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateStartPoint(QPointF)));
                Graph->addItem(a);
                a->UpdateStartPoint(me->pos());
                a->UpdateEndPoint(node->pos());
            }
            if(me->getRelation() == node->getRelation())
            {
                //otherwise check if it's my father in the same relation
                if(parent && parent->getPath() == node->getPath().toStdString()) {
                    Arc* a=new Arc(12,Radius,color,0,0,parentRelation,false);
                    connect(node,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateEndPoint(QPointF)));
                    connect(me,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateStartPoint(QPointF)));
                    a->UpdateEndPoint(node->pos());
                    delete parent;
                    parent = nullptr;
                }
                //and check if it's my prev in the same relations
                //not in exclusive else because don t know if it's impossible to happen in speect HRG
                if(previous && previous->getPath() == node->getPath().toStdString()) {
                    Arc* a=new Arc(12,Radius,color,1,0,parentRelation,false);
                    connect(node,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateEndPoint(QPointF)));
                    connect(me,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateStartPoint(QPointF)));
                    a->UpdateEndPoint(node->pos());
                    delete previous;
                    previous = nullptr;
                }
            }
        }
        //if exist add the next nodes to the printed items and to the one that must be checked
        if(next)
        {
            tbc.push_front(next);
            Node* temp=new Node(QString::fromStdString(next->getPath()),relation,QString::fromStdString(next->getName()),me->pos().x()+4*Radius,me->pos().y(),Radius,color,parentRelation);
            Printed.insert(QString::fromStdString(next->getId()), temp);
            temp->clearFocus();
            Arc* a=new Arc(12,Radius,color,1,1,parentRelation,false);
            connect(temp,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateEndPoint(QPointF)));
            connect(me,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateStartPoint(QPointF)));

            PositionNode(*temp);
            a->UpdateEndPoint(temp->pos());
        }
        //if exist add the daughter to the printed items and to the one that must be checked
        if(child)
        {
            tbc.push_front(child);
            Node* temp=new Node(QString::fromStdString(child->getPath()),relation,QString::fromStdString(child->getName()),me->pos().x(),me->pos().y()+4*Radius,Radius,color,parentRelation);
            Printed.insert(QString::fromStdString(child->getId()),temp);
            temp->clearFocus();
            Arc* a=new Arc(12,Radius,color,0,1,parentRelation,false);
            connect(temp,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateEndPoint(QPointF)));
            connect(me,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateStartPoint(QPointF)));
            PositionNode(*temp);
            a->UpdateEndPoint(temp->pos());
        }
        me->notifyPositionChange(me->pos());
    }
}

/*
 * Description: changes the visibility of the relations in the graph
 * @param QStandardItem * - Qt standard item as relation reference (see Qt docs for more info)
 * @return void
 */
void GraphManager::changeRelationVisibility(QStandardItem *key)
{
    //find the item that represent the relation in the graph model and hide it
    auto it=Relations.find(key->text());
    if(it!=Relations.end())
        (*it)->setVisible(!(*it)->isVisible());
}

/*
 * Description: notifies the selection/deselection of a item
 * @return void
 */
void GraphManager::notifySelection()
{
    QList<QGraphicsItem*> selected(Graph->selectedItems());
    if(!selected.empty())
    {
        auto myNode=std::find(Printed.begin(),Printed.end(),Graph->focusItem());
        if(myNode!=Printed.end())
        {
            focusSignal((*myNode)->getRelation(),(*myNode)->getPath(),true);
        }
    }
    else{
        cleardetails();
    }
}

/*
 * Description: focuses the node given the relation and the path
 * @param const QString& - Qt string as relation name (see Qt docs for more info) 
 * @param const QString& - Qt string as path (see Qt docs for more info) 
 * @return void
 */
void GraphManager::selectItem(const QString &relation, const QString &path)
{
    ID searchedItem(path,relation);
    foreach(Node* item,Printed)
    {
        if((*item)==searchedItem)
            item->setFocus();
    }
}
