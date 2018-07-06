#include "graphmanager.hpp"
#include <QFocusEvent>
#include <QGraphicsItemGroup>
#include <QGraphicsView>
#include "arc.hpp"
#include "DSItem.hpp"
#include "node.hpp"


GraphManager::GraphManager():Graph(new QGraphicsScene()),Relations(),RelationsModel(new QStandardItemModel()) {
    //when an item in the relations model get checked or unchecked graph manager will tell to the graph to hide or show the relation
    connect(RelationsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(changeRelationVisibility(QStandardItem*)));
    connect(Graph,SIGNAL(selectionChanged()),this,SLOT(notifySelection()));
}


GraphManager::~GraphManager(){
    clear();
    delete Graph;
    delete RelationsModel;
}


void GraphManager::linkGraphModel(QGraphicsView* v){
    v->setScene(Graph);
}


void GraphManager::linkRelationModel(QListView* v){
    v->setModel(RelationsModel);
}


bool GraphManager::printRelation(const QString &id, const DSItem *SpeectNode, const QColor &Color) {
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


#include "iostream"
void GraphManager::clear() {
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

QGraphicsRectItem *GraphManager::generateRelation(const QString &id, const QColor &color) {

    if(!Relations.contains(id)) {
        RelationsModel->appendRow(generateItem(id,color));
        QGraphicsRectItem* t=new QGraphicsRectItem();
        Relations.insert(id,t);
        return t;
    }
    return NULL;

}

QStandardItem *GraphManager::generateItem(const QString &id,const QColor&color) {
    QStandardItem* item=new QStandardItem(id);
    item->setBackground(QBrush(color));
    item->setCheckable(true);
    item->setEditable(false);
    item->setCheckState(Qt::Checked);
    item->setSelectable(false);
    return item;
}


void GraphManager::PositionNode(Node &me) {
    while(me.collidingItems().size()>1){
        me.setX(me.x()+(4*Radius));
    }
}


void GraphManager::FixHeadPosition(Node &me) {
    while(me.collidingItems().size()>1){
        me.setY(me.y()+(4*Radius));
    }
}

void GraphManager::checkRelations(QVector<const DSItem*> &tbc, const QString& relation, const QColor& color, QGraphicsItem *parentRelation) {
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
            if(me->getRelation() == node->getRelation())
            {
                //otherwise check if it's my father in the same relation
                if(parent && parent->getPath() == node->getPath().toStdString()) {
                    Arc* a=new Arc(10,Radius,color,0,0,parentRelation,false);
                    connect(node,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateEndPoint(QPointF)));
                    connect(me,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateStartPoint(QPointF)));
                    a->UpdateEndPoint(node->pos());
                    delete parent;
                    parent = nullptr;
                }
                //and check if it's my prev in the same relations
                //not in exclusive else because don t know if it's impossible to happen in speect HRG
                if(previous && previous->getPath() == node->getPath().toStdString()) {
                    Arc* a=new Arc(10,Radius,color,0,0,parentRelation,false);
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
            Arc* a=new Arc(10,Radius,color,0,1,parentRelation,false);
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
            Arc* a=new Arc(10,Radius,color,0,1,parentRelation,false);
            connect(temp,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateEndPoint(QPointF)));
            connect(me,SIGNAL(notifyPositionChange(QPointF)),a,SLOT(UpdateStartPoint(QPointF)));
            PositionNode(*temp);
            a->UpdateEndPoint(temp->pos());
        }
        me->notifyPositionChange(me->pos());
    }
}

void GraphManager::changeRelationVisibilityList(QStringList allKeys,QStringList checkedKeys) {

    // start with clean state: hide all the Relations

    for(int i = 0; i < allKeys.length();++i){
        auto it = Relations.find(allKeys.at(i));
        if(it != Relations.end()) {
            (*it)->setVisible(false);
        }
    }

    // show only checked relations
    // unchecked ones remain hidden

    for(int i = 0; i < checkedKeys.length();++i){
        auto it = Relations.find(checkedKeys.at(i));
        if(it != Relations.end())
            (*it)->setVisible(true);
    }
}





/*
 * Description: changes the visibility of the relations in the graph
 * @param QStandardItem * - Qt standard item as relation reference (see Qt docs for more info)
 * @return void
 */
void GraphManager::changeRelationVisibility(QStandardItem *key) {
    //find the item that represent the relation in the graph model and hide it
    auto it=Relations.find(key->text());
    if(it!=Relations.end())
        (*it)->setVisible(!(*it)->isVisible());
}


void GraphManager::notifySelection() {
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


void GraphManager::selectItem(const QString &relation, const QString &path) {
    ID searchedItem(path,relation);
    foreach(Node* item,Printed)
    {
        if((*item)==searchedItem)
            item->setFocus();
    }
}
