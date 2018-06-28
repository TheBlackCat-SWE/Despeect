#include "DSRelationControlDockWidget.hpp"


void DSRelationControlDockWidget::setupUI() {

    doConnections();

    list_view->setModel(rel_list_model);
    v_layout->addWidget(list_view);
    v_layout->addLayout(h_layout);
    h_layout->addWidget(show_all_button);

    QWidget* internalWidget = new QWidget();
    internalWidget->setLayout(v_layout);
    setWidget(internalWidget);
    setWindowTitle("Relations");
}

void DSRelationControlDockWidget::fillQueue() {


}

void DSRelationControlDockWidget::doConnections() {


}

void DSRelationControlDockWidget::showAllClicked() {


}

void DSRelationControlDockWidget::showSingleClicked() {

}

void DSRelationControlDockWidget::fetchData() {
    rel_list_model->fetchData();
}


DSRelationControlDockWidget::DSRelationControlDockWidget(QWidget* parent,DSAdapter* adapter) :
    QDockWidget (parent),
    rel_list_model(new DSRelListModel(this,adapter)),
    list_view(new QListView(this)),
    v_layout(new QVBoxLayout()),
    h_layout(new QHBoxLayout()),
    show_all_button(new QPushButton("Show All"))
{
    setupUI();
}
