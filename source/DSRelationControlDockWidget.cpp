#include "DSRelationControlDockWidget.hpp"


void DSRelationControlDockWidget::setupUI() {

    //start view model definition
    list_view->setModel(rel_list_model);

    // end view model definition

    // start widget definition

    viewBox = new QGroupBox;
    buttonBox = new QDialogButtonBox;

    // end widget definition

    // start create Layout

    QVBoxLayout* viewLayout = new QVBoxLayout;
    viewLayout->addWidget(list_view);
    viewBox->setLayout(viewLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(viewBox);

    QWidget* internalWidget = new QWidget();
    internalWidget->setLayout(mainLayout);
    setWidget(internalWidget);
    setWindowTitle("Relation List");

    // end create Layout

    // start connecting all button with slots

    doConnections();

    // end connecting all button with slots
    updateAvailableRelations();
}


void DSRelationControlDockWidget::doConnections() {

    QObject::connect(rel_list_model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(showAll()));

}


void DSRelationControlDockWidget::showAll() {

    QStringList checkedRelations = rel_list_model->getCheckedRelations();
    QStringList allRelations = rel_list_model->getAllRelations();

    emit showRelation(allRelations,checkedRelations);

}

void DSRelationControlDockWidget::updateAvailableRelations() {
    rel_list_model->fetchData();
}

DSRelationControlDockWidget::DSRelationControlDockWidget(QWidget* parent,DSAdapter* adapter)
    :QDockWidget (parent)
{
    list_view = new QListView;
    rel_list_model = new DSRelListModel(this,adapter);
    setupUI();
}
