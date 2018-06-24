#ifndef VSCENE_H
#define VSCENE_H

#include "DSItem.hpp"
#include "VNode.hpp"
#include <QGraphicsScene>
#include <set>

class DSAdapter;

class VScene : public QGraphicsScene {
private:
    DSAdapter* adapter;
    std::vector<VNode*> nodes;
public:
    VScene(DSAdapter* adapter, QObject* parent);
    void printRels();
    void printRel(std::string name, QColor color);
    void showGraph();

    void addVNode(int x, int y, DSItem* n, QColor c);
    void printArcs(VNode* n);
    void printArc(VNode* start, DSItem* end);

    void deleteGraph();
};

#endif // VSCENE_H
