/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSMainWindow.hpp"
#include "DSAdapter.hpp"
#include "DSListModel.hpp"
#include "DSRelation.hpp"
#include <QIcon>
#include <QMenu>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <string>
#include <QDebug>
#include <iostream>
#include <QInputDialog>
#include <QLineEdit>

void DSMainWindow::setupLog(){
    logFile->remove();
    stderr=freopen("log.txt","a",stderr);
}

void DSMainWindow::createActions() {
    actions["loadVoiceAct"] = new QAction(QIcon::fromTheme("document-open"), "Load Voice File", this);
    actions["showVoicePathAct"] = new QAction(QIcon::fromTheme("dialog-information"), "Show Voice Path", this);
    actions["selectNodeFromPath"] = new QAction(QIcon::fromTheme("dialog-information"),"Insert Path To Node", this);
    actions["exportGraph"] = new QAction(QIcon::fromTheme("dialog-information"),"Export Graph", this);
}

void DSMainWindow::createMenus() {
        QMenu* fileMenu = new QMenu("File", this);
        QList<QString> action_key = actions.keys();
        for(int i = 0; i < action_key.size(); i++) {
            fileMenu->addAction(actions[action_key[i]]);
            // if(i == 2 || i == 6) fileMenu->addSeparator();
        }
        //fileMenu->addSeparator();
        menu_bar->addMenu(fileMenu);
}

void DSMainWindow::doConnections() {
    connect(actions["loadVoiceAct"], &QAction::triggered, this, &DSMainWindow::loadVoice);
    connect(actions["showVoicePathAct"], &QAction::triggered, this, &DSMainWindow::showVoicePath);
    connect(actions["exportGraph"], &QAction::triggered, this, &DSMainWindow::exportGraph);
    connect(actions["selectNodeFromPath"], &QAction::triggered, this, &DSMainWindow::selectNodeFromPath);
    connect(graph_manager->Graph, &QGraphicsScene::selectionChanged, this, &DSMainWindow::showNodeFeatures);
    connect(run_feat_proc, &QPushButton::clicked, this, &DSMainWindow::execFeatProc);
    connect(this, &DSMainWindow::fetchData, flow_dock, &DSFlowControlDockWidget::fetchData);
    connect(this, &DSMainWindow::fetchData, list_model, &DSListModel::fetchData);
    connect(flow_dock, &DSFlowControlDockWidget::execUttProc, this, &DSMainWindow::execUttProc);
    connect(flow_dock, &DSFlowControlDockWidget::execUttProcList, this, &DSMainWindow::execUttProcList);
    connect(flow_dock, &DSFlowControlDockWidget::resetUtterance, this, &DSMainWindow::resetUtterance);
    connect(rel_dock,&DSRelationControlDockWidget::showRelation,this,&DSMainWindow::showRelations);
    connect(text_dock, &DSTextDockWidget::loadButtonClicked, this, &DSMainWindow::loadTextFromFile);
}
/*
void DSMainWindow::setupSB(){
    std::stringstream buff;
    std::streambuf * old = std::cerr.rdbuf(buff.rdbuf());
    std::cerr<<"test";
    std::string text = buff.str();
    QString Qtext=QString::fromStdString(text);
    status_bar->showMessage(Qtext);
}
*/
void DSMainWindow::setupUI() {
    list_view->setModel(list_model);
    QVBoxLayout* feat_proc_layout=new QVBoxLayout;
    feat_proc_layout->addWidget(list_view);
    feat_proc_layout->addWidget(run_feat_proc);

    QWidget* internal_widget = new QWidget();
    internal_widget->setLayout(feat_proc_layout);
    list_dock->setWidget(internal_widget);

    addDockWidget(Qt::RightDockWidgetArea, table_dock);
    addDockWidget(Qt::LeftDockWidgetArea, flow_dock);
    addDockWidget(Qt::TopDockWidgetArea, text_dock);
    addDockWidget(Qt::LeftDockWidgetArea, list_dock);
    addDockWidget(Qt::RightDockWidgetArea,rel_dock);
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    setCentralWidget(graph_view);
    setMenuBar(menu_bar);
    //tool_bar->setMovable(false);
    //addToolBar(Qt::TopToolBarArea, tool_bar);
    status_bar->setSizeGripEnabled(false);
    status_bar->setStyleSheet("color: red");
    setStatusBar(status_bar);
    graph_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    createActions();
    createMenus();
    doConnections();
}

void DSMainWindow::exportGraph(){
    graph_manager->Graph->clearSelection();                                                  // Selections would also render to the file
    graph_manager->Graph->setSceneRect(graph_manager->Graph->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
    QImage image(graph_manager->Graph->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
    image.fill(Qt::white);                                              // Start all pixels white

    QPainter painter(&image);
    graph_manager->Graph->render(&painter);
    image.save("graph.png");
}

void DSMainWindow::loadVoice() {
    voice_path = QFileDialog::getOpenFileName(this, "Open Configuration File",
                                              "../../", "Voice Files (*.json)");
    if(!voice_path.isEmpty()) {
        adapter->loadVoice(voice_path.toStdString());
        emit fetchData();
        }

}

void DSMainWindow::showVoicePath() {
    QMessageBox msgBox;
    msgBox.setText(voice_path);
    msgBox.exec();
}

void DSMainWindow::selectNodeFromPath() {
    status_bar->clearMessage();
    graph_manager->Graph->setFocus();
    if(graph_manager->Graph->focusItem()){
        auto myNode=std::find(graph_manager->Printed.begin(),graph_manager->Printed.end(),graph_manager->Graph->focusItem());
        QString text = QInputDialog::getText(this, (*myNode)->getRelation(),
                                            (*myNode)->getPath(), QLineEdit::Normal);
        if(!text.isNull()) {
            QString realPath((*myNode)->getPath()+text);
            graph_manager->selectItem((*myNode)->getRelation(),realPath);
        }
    } else
        status_bar->showMessage("select a node");

}

void DSMainWindow::showNodeFeatures() {
    status_bar->clearMessage();
    if(graph_manager->Graph->focusItem()){
        auto myNode=std::find(graph_manager->Printed.begin(),graph_manager->Printed.end(),graph_manager->Graph->focusItem());
        QMap<std::string,std::string> feat=(*myNode)->getFeatures();
        QStandardItemModel* table_model = new QStandardItemModel(feat.size(), 2);
        auto i=feat.begin();
        int row=0;
        while(i!=feat.end()){
            QStandardItem *itemKey = new QStandardItem(QString(i.key().c_str()));
            table_model->setItem(row, 0, itemKey);
            QStandardItem *itemValue = new QStandardItem(QString(i.value().c_str()));
            table_model->setItem(row, 1, itemValue);
            row++;
            ++i;
        }
        featuresTable->setModel(table_model);
        table_dock->setWidget(featuresTable);
        featuresTable->resizeColumnsToContents();
    } else
        status_bar->showMessage("select a node");
}

void DSMainWindow::execFeatProc() {
    s_erc error = S_SUCCESS;
    std::string feature;
    status_bar->clearMessage();
    graph_manager->Graph->setFocus();
    if(graph_manager->Graph->focusItem()){
        auto myNode=std::find(graph_manager->Printed.begin(),graph_manager->Printed.end(),graph_manager->Graph->focusItem());
        QModelIndexList index=list_view->selectionModel()->selectedIndexes();
        const char* key=list_model->data(index[0]).toString().toStdString().c_str();
        SObject* obj=adapter->execFeatProcessor(key,(*myNode)->getSItem());
        if(SObjectIsType(obj,"SString",&error)) {
            std::string value(SObjectGetString(obj,&error));
            feature=value;
        } else
        if(SObjectIsType(obj,"SInt",&error)) {
            sint32 value(SObjectGetInt(obj, &error));
            feature=std::to_string(value);
        } else
        if(SObjectIsType(obj,"SFloat",&error)) {
            float value(SObjectGetFloat(obj, &error));
            feature=std::to_string(value);
        }
        if(feature.empty()){
            QMessageBox msgBox;
            msgBox.setText(QString::fromStdString("Selected feature does not exist"));
            msgBox.exec();
        }else{
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(feature));
        msgBox.exec();
            }
        }
        else
            status_bar->showMessage("select a node");
}


void DSMainWindow::loadTextFromFile() {
    QString file_path = QFileDialog::getOpenFileName(this, "Open Text File",
                                                     "../../", "Text Files (*.txt)");
    QFile file(file_path);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream read_file(&file);
    text_dock->setText(read_file.readAll());
}

void DSMainWindow::execUttProc(std::string utt_proc) {
    loadText();
    adapter->execUttProc(utt_proc);
    int i=0;
    graph_manager->clear();
    foreach (auto t,adapter->getRelList())
    {
        const DSRelation* currentRelation = adapter->getRel(t);
        DSItem* temp = currentRelation->getHead();
        graph_manager->printRelation(QString(t.c_str()), temp, colors.at(i%colors.size()));
        delete currentRelation;
        ++i;
    }
/*
    std::stringstream buff;
    std::streambuf * old = std::cerr.rdbuf(buff.rdbuf());
    std::cerr<<s_error_str(adapter->getError());
    std::string text = buff.str();
    QString Qtext=QString::fromStdString(text);
    status_bar->showMessage(Qtext); */
    // for relation
    emit updateAvailableRelations();
}


void DSMainWindow::execUttProcList(const std::vector<std::string> &proc_list) {
    loadText();
    adapter->execUttProcList(proc_list);

    int i=0;
    graph_manager->clear();
    foreach (auto t,adapter->getRelList())
    {
        const DSRelation* currentRelation = adapter->getRel(t);
        DSItem* temp = currentRelation->getHead();
        if(temp)
            graph_manager->printRelation(QString(t.c_str()), temp, colors.at(i%colors.size()));
        ++i;
    }
    // for relation
    emit updateAvailableRelations();

}

void DSMainWindow::updateAvailableRelations(){
    rel_dock->updateAvailableRelations();

}

void DSMainWindow::showRelations(QStringList allKeys,QStringList checkedKeys) {
    graph_manager->changeRelationVisibilityList(allKeys,checkedKeys);
}


void DSMainWindow::resetUtterance() {
    adapter->resetUtterance();
    graph_manager->clear();
    // for relation
    emit updateAvailableRelations();

}

DSMainWindow::DSMainWindow(QWidget* parent):
    QMainWindow(parent),
    adapter(DSAdapter::createAdapter()),
    list_model(new DSListModel(this, adapter)),
    list_view(new QListView(this)),
    flow_dock(new DSFlowControlDockWidget(this, adapter)),
    rel_dock(new DSRelationControlDockWidget(this,adapter)),
    text_dock(new DSTextDockWidget(this)),
    list_dock(new QDockWidget("Feature Processor", this)),
    table_dock(new QDockWidget("Selected node features", this)),
    graph_manager(new GraphManager()),
    featuresTable(new QTableView()),
    graph_view(new QGraphicsView(this)),
    run_feat_proc(new QPushButton("Run",this)),
    menu_bar(new QMenuBar(this)),
    status_bar(new QStatusBar(this)),
    logFile(new QFile("log.txt"))
{
    setupLog();
    setupUI();
    // setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
    //                                 qApp->desktop()->availableGeometry()));

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Despeect");
    graph_manager->linkGraphModel(graph_view);


    //this is the relations colors after 10 relation start from beginning
    colors.push_back(QColor(qRgb(213,0,0)));
    colors.push_back(QColor(qRgb(120,144,156)));
    colors.push_back(QColor(qRgb(170,0,255)));
    colors.push_back(QColor(qRgb(109,76,65)));
    colors.push_back(QColor(qRgb(251,140,0)));
    colors.push_back(QColor(qRgb(67,160,61)));
    colors.push_back(QColor(qRgb(41,98,255)));
    colors.push_back(QColor(qRgb(255,214,0)));
    colors.push_back(QColor(qRgb(0,184,212)));
    colors.push_back(QColor(qRgb(0,191,165)));
}

DSMainWindow::~DSMainWindow() {
    delete adapter;
}

void DSMainWindow::loadText() {
    adapter->loadText(text_dock->getText().toStdString());
}
