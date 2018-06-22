/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSMainWindow.hpp"
#include "DSAdapter.hpp"
#include "DSListModel.hpp"
#include "DSCentralWidget.hpp"
#include <QIcon>
#include <QMenu>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <string>


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
}

void DSMainWindow::setupUI() {
    list_view->setModel(list_model);
    list_dock->setWidget(list_view);
    addDockWidget(Qt::LeftDockWidgetArea, flow_dock);
    addDockWidget(Qt::LeftDockWidgetArea, list_dock);
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    setCentralWidget(central_widget);
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
        /*
             * Prevents the models from fetching data linked to the voice before loading voice.json
             * resulting in an internal speect error.
             */
        flow_dock->fetchData();
        list_model->fetchData();
    }
}

void DSMainWindow::showVoicePath() {
    QMessageBox msgBox;
    msgBox.setText(voice_path);
    msgBox.exec();
}

DSMainWindow::DSMainWindow(QWidget* parent):
    QMainWindow(parent),
    adapter(DSAdapter::createAdapter()),
    list_model(new DSListModel(this, adapter)),
    list_view(new QListView(this)),
    flow_dock(new DSFlowControlDockWidget(this, adapter)),
    list_dock(new QDockWidget("Feature Processor", this)),
    central_widget(new DSCentralWidget(this, adapter)),
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
