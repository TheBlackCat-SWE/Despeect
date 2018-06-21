/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSMainWindow.hpp"
#include "DSAdapter.hpp"
#include "DSTreeModel.hpp"
#include "DSListModel.hpp"
#include "DSCentralWidget.hpp"
#include <string>
#include <QFileDialog>
#include <QMessageBox>


void DSMainWindow::createActions() {
    /* EXAMPLE CODE
        list->append(new QAction(QIcon(":/resources/icons/exec.png"), "Esegui", this)); //[0] execAct
        for(int i = 0; i < list->size(); i++) tool_bar->addAction(list->at(i));
        tool_bar->insertSeparator(list->at(n));
        */
    createActionLoadVoice();
    createActionShowVoicePath();
}

void DSMainWindow::createMenus() {
        QMenu* fileMenu = new QMenu("File", this);
        for(int i = 0; i < actions.size(); i++) {
            fileMenu->addAction(actions.at(i));
            // if(i == 2 || i == 6) fileMenu->addSeparator();
        }
        fileMenu->addSeparator();

        QAction* exitAct = new QAction(QIcon(":/Risorse/Icons/exit.png"), "Esci", this);
        fileMenu->addAction(exitAct);
        connect(exitAct, &QAction::triggered, this, &DSMainWindow::close);

        menu_bar->addMenu(fileMenu);
}

void DSMainWindow::doConnections() {
    // connect(list->at(0), &QAction::triggered, /*Widget Pointer*/, /*Widget Slot*/);
}

void DSMainWindow::setupUI() {
    tree_view->setModel(tree_model);
    tree_dock->setWidget(tree_view);
    list_view->setModel(list_model);
    list_dock->setWidget(list_view);
    addDockWidget(Qt::LeftDockWidgetArea, tree_dock);
    addDockWidget(Qt::LeftDockWidgetArea, list_dock);
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    setCentralWidget(central_widget);
    tool_bar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, tool_bar);
    status_bar->setSizeGripEnabled(false);
    status_bar->setStyleSheet("color: red");
    setStatusBar(status_bar);

    createActions();
    createMenus();
    doConnections();
}

DSMainWindow::DSMainWindow(QWidget* parent):
    QMainWindow(parent),
    adapter(DSAdapter::createAdapter()),
    tree_model(new DSTreeModel(this, adapter)),
    tree_view(new QTreeView(this)),
    list_model(new DSListModel(this, adapter)),
    list_view(new QListView(this)),
    tree_dock(new QDockWidget("Utterance Type", this)),
    list_dock(new QDockWidget("Feature Processor", this)),
    central_widget(new DSCentralWidget(this, adapter)),
    tool_bar(new QToolBar("Barra Degli Strumenti", this)),
    menu_bar(new QMenuBar(this)),
    status_bar(new QStatusBar(this))
{
    /*
    std::string text =
            "Hello Judith. How are you?";

    std::string audio_output_path =
            "/home/ricc/Workspace-QtCreator/Despeect/output.wav";
    */
 /*   if(adapter)
        if(adapter->loadInputText(text))
            adapter->synthesize();*/
            // if(adapter->synthesize()) adapter->saveOutputAudio(audio_output_path);

    setupUI();
    // setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
    //                                 qApp->desktop()->availableGeometry()));
    //Toglie il flag usando le operazioni bitwise (AND e NOT)
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Despeect");
}

void DSMainWindow::createActionLoadVoice() {
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));

    QAction* openAct = new QAction(openIcon, QObject::tr("Load Configuration File"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Load Configuration File"));
    QObject::connect(openAct, &QAction::triggered, [=] () {
        voice_path = QFileDialog::getOpenFileName(this, QObject::tr("Oper Configuration File"),
                                                  "../../", QObject::tr("Voice Files (*.json)"));
        if(!voice_path.isEmpty()) {
            adapter->loadVoice(voice_path.toStdString());
            /*
             * Prevents the models from fetching data linked to the voice before loading voice.json
             * resulting in an internal speect error.
             */
            tree_model->fetchData();
            list_model->fetchData();
        }
    });
    actions.push_back(openAct);
}

void DSMainWindow::createActionShowVoicePath() {
    const QIcon openIcon = QIcon::fromTheme("dialog-information");
    QAction* openAct = new QAction(openIcon, QObject::tr("Show Voice Path"), this);
    openAct->setStatusTip(tr("Show Voice Path"));
    QObject::connect(openAct, &QAction::triggered, [this] () {
        QMessageBox msgBox;
        msgBox.setText(voice_path);
        msgBox.exec();
    });
    actions.push_back(openAct);
}

DSMainWindow::~DSMainWindow() {
    delete adapter;
}
