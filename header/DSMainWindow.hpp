/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#ifndef DSMAINWINDOW_H
#define DSMAINWINDOW_H

#include "DSTreeDockWidget.hpp"
#include <QMainWindow>
#include <QString>
#include <QTreeView>
#include <QListView>
#include <QDockWidget>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>
#include <QVector>


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
    DSTreeDockWidget* tree_dock;
    QDockWidget* list_dock;
    DSCentralWidget* central_widget;
    QToolBar* tool_bar;
    QMenuBar* menu_bar;
    QStatusBar* status_bar;
    QVector<QAction*> actions;

    void createActions();
    void createMenus();
    void doConnections();
    void setupUI();

    void createActionLoadVoice();
    void createActionShowVoicePath();
public:
    DSMainWindow(QWidget* parent = Q_NULLPTR);
    ~DSMainWindow();
};

#endif // DSMAINWINDOW_H
