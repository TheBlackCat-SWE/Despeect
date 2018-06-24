/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSMainWindow.hpp"
#include "DSAdapter.hpp"
#include "DSListModel.hpp"
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
    connect(flow_dock, &DSFlowControlDockWidget::resetUtterance, this, &DSMainWindow::resetUtterance);
    connect(text_dock, &DSTextDockWidget::loadButtonClicked, this, &DSMainWindow::loadTextFromFile);
}

void DSMainWindow::setupUI() {
    list_view->setModel(list_model);
    list_dock->setWidget(list_view);
    addDockWidget(Qt::LeftDockWidgetArea, flow_dock);
    addDockWidget(Qt::TopDockWidgetArea, text_dock);
    addDockWidget(Qt::LeftDockWidgetArea, list_dock);
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    setCentralWidget(graph_view);
    setMenuBar(menu_bar);
    //tool_bar->setMovable(false);
    //addToolBar(Qt::TopToolBarArea, tool_bar);
    //status_bar->setSizeGripEnabled(false);
    //status_bar->setStyleSheet("color: red");
    //setStatusBar(status_bar);

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

void DSMainWindow::execUttProc(const std::vector<std::string> &proc_list) {
    loadText();

    qDebug()<<proc_list.size(); // NON FUNZIONA PASSAGGIO della lista

//  qDebug()<<(adapter->execUttProc("Tokenize"));
    std::vector<std::string> procs = adapter->getUttProcList();
 /*   for(auto it= procs.begin(); it != procs.end(); it++)
        qDebug()<<QString::fromStdString(*it);*/

    qDebug()<< adapter->execUttProcList(proc_list);
//  qDebug()<< adapter->execUttProc(procs.at(1));
//  qDebug()<< adapter->execUttProc(procs.at(1));

    graph_scene->showGraph();
}

void DSMainWindow::resetUtterance() {
    adapter->resetUtterance();
    graph_scene->deleteGraph();
}

DSMainWindow::DSMainWindow(QWidget* parent):
    QMainWindow(parent),
    adapter(DSAdapter::createAdapter()),
    list_model(new DSListModel(this, adapter)),
    list_view(new QListView(this)),
    flow_dock(new DSFlowControlDockWidget(this, adapter)),
    text_dock(new DSTextDockWidget(this)),
    list_dock(new QDockWidget("Feature Processor", this)),
    graph_scene(new VScene(adapter, this)),
    graph_view(new QGraphicsView(graph_scene, this)),
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
}

DSMainWindow::~DSMainWindow() {
    delete adapter;
}

void DSMainWindow::loadText() {
    adapter->loadText(text_dock->getText().toStdString());
}
