/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSCentralWidget.hpp"
#include "DSRelation.hpp"
#include "DSItem.hpp"
#include "VNode.hpp"
#include "VScene.hpp"

#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <cassert>
#include <QLineEdit>

DSCentralWidget::DSCentralWidget(QWidget *parent, DSAdapter *adapter):
    QWidget(parent),
    text(new QLineEdit(this)),
    btn(new QPushButton("Genera Grafo", this)),
    adapter(adapter)
{
    scene = new VScene(adapter, this);
    view = new QGraphicsView(scene, this);

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(text);
    grid->addWidget(btn);
    grid->addWidget(view);
    setLayout(grid);

    text->setPlaceholderText("Inserire il testo");

    QObject::connect(btn,SIGNAL(clicked(bool)), this, SLOT(showGraph(bool)));
}

void DSCentralWidget::showGraph(bool) {
    adapter->loadInputText(text->text().toStdString());
    adapter->synthesize();
    scene->showGraph();
}
