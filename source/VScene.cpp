#include "VScene.hpp"
#include "DSAdapter.hpp"
#include "DSRelation.hpp"
#include "VNode.hpp"
#include <QPainter>

VScene::VScene(DSAdapter* adapter, QObject* parent):
    QGraphicsScene(parent),
    adapter(adapter),
    nodes(std::vector<VNode*>())
{
}

void VScene::printRels(){
    std::vector<std::string> rels = adapter->getRelList();
    QString str=QString();
    for (auto it = rels.begin() ; it != rels.end(); ++it) {
        str+=((*it).c_str());
        str+= "\n";
    }
    QGraphicsTextItem * io = new QGraphicsTextItem;
    io->setPos(-100,-100);
    io->setPlainText(str);
    addItem(io);
}

void VScene::addVNode(int x, int y, DSItem* dnode, QColor c) {
    VNode* node = new VNode(x, y, dnode, c);
    addItem(node);
    nodes.push_back(node);
}

void VScene::printArc(VNode* start, DSItem* end) {
    if(end) {

        VNode* e=nullptr;
        for(auto it=nodes.begin(); it != nodes.end(); ++it) {
            if(*((*it)->info) == *end)
                e=*it;
        }
        QLineF line = QLineF(start->getCenter(), e->getCenter());
        addLine(line,QPen(Qt::blue));
    }
}

void VScene::printArcs(VNode* n){
    printArc(n, n->info->previous());
    printArc(n, n->info->next());
    printArc(n, n->info->parent());
    printArc(n, n->info->child());
}

void VScene::printRel(std::string name, QColor color) {
    DSRelation* rel = adapter->getRel(name);

    //nodo di partenza
    DSItem* dnode = rel->getHead();
    QRectF r = this->sceneRect();
    int pY, mY, dY, ddY;
    int pX=0, x=0, dX=0, ddX=0;

    if(dnode->parent()) {
        pY = r.bottom() + 30;
        mY = r.bottom() + 100;
        dY = r.bottom() + 170;
        ddY = r.bottom() + 240;
    }else {
        pY = 0;
        mY = r.bottom() + 30;
        dY = r.bottom() + 100;
        ddY = r.bottom() + 170;
    }

    while(dnode != nullptr) {

        DSItem* parent = dnode->parent();
        while(parent != nullptr) {
            addVNode(pX, pY, parent,color);
            parent = parent->next();
            pX+=80;
        }
        addVNode(x,mY,dnode, color);
        DSItem* child = dnode->child();
        while(child != nullptr) {
            dX = dX>=x ? dX : x;
            addVNode(dX, dY, child,color);

            DSItem* cc = child->child();
            while(cc != nullptr) {
                ddX = ddX>=dX ? ddX : dX;
                addVNode(ddX, ddY, cc,color);
                cc = cc->next();
                ddX +=80;
            }

            child = child->next();
            dX+=80;
        }
        dnode = dnode ->next();
        x+=80;
    }
}

void VScene::showGraph() {
    std::vector<std::string> rels = adapter->getRelList();
    for(auto it = rels.begin(); it != rels.end(); it++)
       printRel(*it, Qt::red);
  /*  printRel(rels[0], Qt::red);
    printRel(rels[1], Qt::green);
    printRel(rels[2], Qt::cyan);
    printRel(rels[3], Qt::blue);
    printRel(rels[4], Qt::white);
    printRel(rels[5], Qt::yellow);
    printRel(rels[6], Qt::black);*/

    for(auto it = nodes.begin(); it != nodes.end(); it++) {
        printArcs((*it));
    }
}
