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


void DSMainWindow::createActions() {
    actions["loadVoiceAct"] = new QAction(QIcon::fromTheme("document-open"), "Load Voice File", this);
    actions["showVoicePathAct"] = new QAction(QIcon::fromTheme("dialog-information"), "Show Voice Path", this);
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
    connect(this, &DSMainWindow::fetchData, flow_dock, &DSFlowControlDockWidget::fetchData);
    connect(this, &DSMainWindow::fetchData, list_model, &DSListModel::fetchData);
    connect(flow_dock, &DSFlowControlDockWidget::execUttProc, this, &DSMainWindow::execUttProc);
    connect(flow_dock, &DSFlowControlDockWidget::execUttProcList, this, &DSMainWindow::execUttProcList);
    connect(flow_dock, &DSFlowControlDockWidget::resetUtterance, this, &DSMainWindow::resetUtterance);
    connect(rel_dock,&DSRelationControlDockWidget::showAll,this,&DSMainWindow::showRelation);
    connect(rel_dock,&DSRelationControlDockWidget::showAll,this,&DSMainWindow::showAllRelation);
    connect(text_dock, &DSTextDockWidget::loadButtonClicked, this, &DSMainWindow::loadTextFromFile);
}

void DSMainWindow::setupUI() {
    list_view->setModel(list_model);
    list_dock->setWidget(list_view);
    addDockWidget(Qt::LeftDockWidgetArea, flow_dock);
    addDockWidget(Qt::TopDockWidgetArea, text_dock);
    addDockWidget(Qt::LeftDockWidgetArea, list_dock);
    addDockWidget(Qt::BottomDockWidgetArea,rel_dock);
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    setCentralWidget(graph_view);
    setMenuBar(menu_bar);
    //tool_bar->setMovable(false);
    //addToolBar(Qt::TopToolBarArea, tool_bar);
    //status_bar->setSizeGripEnabled(false);
    //status_bar->setStyleSheet("color: red");
    //setStatusBar(status_bar);

    graph_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    createActions();
    createMenus();
    doConnections();
}

void DSMainWindow::loadVoice() {
    voice_path = QFileDialog::getOpenFileName(this, "Oper Configuration File",
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

void DSMainWindow::loadTextFromFile() {
    QString file_path = QFileDialog::getOpenFileName(this, "Oper Text File",
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

    // for relation
    emit showRelation();
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
    emit showRelation();
}

void DSMainWindow::showRelation() {
    //rel_dock->u
    rel_dock->showAll();
}

void DSMainWindow::showAllRelation() {
    //rel_dock->showAllClicked();
    rel_dock->showAll();
}//

void DSMainWindow::resetUtterance() {
    adapter->resetUtterance();
    graph_manager->clear();

    // for relation
    emit showRelation();
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
    graph_manager(new GraphManager()),
    graph_view(new QGraphicsView(this)),
    //tool_bar(new QToolBar("Barra Degli Strumenti", this)),
    menu_bar(new QMenuBar(this))
    //status_bar(new QStatusBar(this))
{
    setupUI();
    // setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
    //                                 qApp->desktop()->availableGeometry()));

    //Toglie il flag usando le operazioni bitwise (AND e NOT)
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
