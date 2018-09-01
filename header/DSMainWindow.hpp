/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#ifndef DSMAINWINDOW_H
#define DSMAINWINDOW_H

#include "DSFlowControlDockWidget.hpp"
#include "DSRelationControlDockWidget.hpp"
#include "DSTextDockWidget.hpp"
#include "graphmanager.hpp"
#include <QMainWindow>
#include <QString>
#include <QTreeView>
#include <QListView>
#include <QDockWidget>
//#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QMap>
#include <QAction>
#include <QWidget>
#include <QGraphicsView>
#include <sstream>
#include <QFile>

class DSAdapter;
class DSListModel;

class DSMainWindow: public QMainWindow {
    Q_OBJECT
private:
    QFile* logFile;
    DSAdapter* adapter;
    QString voice_path;
    QTableView* featuresTable;
    DSListModel* list_model;
    QListView* list_view;
    DSFlowControlDockWidget* flow_dock;
    DSRelationControlDockWidget* rel_dock;
    DSTextDockWidget* text_dock;
    QDockWidget* list_dock;
    QDockWidget* table_dock;
    GraphManager* graph_manager;
    QGraphicsView* graph_view;
    QPushButton* run_feat_proc;

    //QToolBar* tool_bar;
    QMenuBar* menu_bar;
    QStatusBar* status_bar;
    QMap<QString, QAction*> actions;

    //Field: list of all colors available to print the graph
    QVector<QColor>colors;

    void createActions();
    void createMenus();
    void doConnections();
    void setupUI();
    void setupLog();
signals:
    /*
     * Prevents the models from fetching data linked to the voice before loading voice.json
     * resulting in an internal speect error.
     */
    void fetchData();
private slots:
    void loadVoice();
    void showVoicePath();
    void selectNodeFromPath();
    void execFeatProc();
    void showNodeFeatures();
    void loadTextFromFile();
    void exportGraph();
    void loadText();
    void execUttProc(std::string utt_proc);
    void execUttProcList(const std::vector<std::string>& proc_list);
    void updateAvailableRelations();
    void showRelations(QStringList,QStringList);
    void resetUtterance();
public:
    DSMainWindow(QWidget* parent = Q_NULLPTR);
    ~DSMainWindow();
};

#endif // DSMAINWINDOW_H
