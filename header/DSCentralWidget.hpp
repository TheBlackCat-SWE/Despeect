/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#ifndef DSCENTRALWIDGET_H
#define DSCENTRALWIDGET_H

#include "DSAdapter.hpp"
#include "VScene.hpp"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>

class DSAdapter;

class DSCentralWidget: public QWidget {
    Q_OBJECT
private:
    QGraphicsView* view;
    QLineEdit* text;
    QPushButton* btn;
    DSAdapter* adapter;

public:
    VScene* scene;
    DSCentralWidget(QWidget* parent, DSAdapter* adapter);
public slots:
    void showGraph(bool);
};

#endif // DSCENTRALWIDGET_H
