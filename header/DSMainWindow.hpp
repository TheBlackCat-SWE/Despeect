/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#ifndef DSMAINWINDOW_H
#define DSMAINWINDOW_H

#include "DSFlowControlDockWidget.hpp"
#include "DSTextDockWidget.hpp"
#include <QMainWindow>
#include <QString>
#include <QTreeView>
#include <QListView>
#include <QDockWidget>
//#include <QToolBar>
#include <QMenuBar>
//#include <QStatusBar>
#include <QMap>
#include <QAction>
#include <QWidget>


class DSAdapter;
class DSListModel;
class DSCentralWidget;

class DSMainWindow: public QMainWindow {
    Q_OBJECT
private:
    DSAdapter* adapter;
    QString voice_path;
    DSListModel* list_model;
    QListView* list_view;
    DSFlowControlDockWidget* flow_dock;
    DSTextDockWidget* text_dock;
    QDockWidget* list_dock;
    DSCentralWidget* central_widget;
    //QToolBar* tool_bar;
    QMenuBar* menu_bar;
    //QStatusBar* status_bar;
    QMap<QString, QAction*> actions;

    void createActions();
    void createMenus();
    void doConnections();
    void setupUI();
signals:
    /*
     * Prevents the models from fetching data linked to the voice before loading voice.json
     * resulting in an internal speect error.
     */
    void fetchData();
private slots:
    void loadVoice();
    void showVoicePath();
    void loadTextFromFile();
    void loadText() {
        //Load text into DSAdapter
    }
    void execUttProc(const std::vector<std::string>& proc_list);
    void resetUtterance();
public:
    DSMainWindow(QWidget* parent = Q_NULLPTR);
    ~DSMainWindow();
};

#endif // DSMAINWINDOW_H
