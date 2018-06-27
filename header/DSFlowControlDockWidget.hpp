/**
  * @author Riccardo Damiani
  * @version 1.0 21/06/18
  */

#ifndef DSFLOWCONTROLDOCKWIDGET_H
#define DSFLOWCONTROLDOCKWIDGET_H

#include "DSTreeModel.hpp"
#include <QDockWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QQueue>
#include <QString>
#include <QWidget>

class DSAdapter;

class DSFlowControlDockWidget: public QDockWidget {
    Q_OBJECT
private:
    DSTreeModel* tree_model;
    QTreeView* tree_view;
    QVBoxLayout* v_layout;
    QHBoxLayout* h_layout;
    QPushButton* run_all_button;
    QPushButton* run_step_button;
    QPushButton* reset_button;
    QQueue<QString> proc_queue;

    void setupUI();
    void fillQueue();
    void doConnections();
signals:
    void execUttProc(std::string);
    void execUttProcList(const std::vector<std::string>& proc_list);
    void resetUtterance();
public slots:
    void run_all_clicked();
    void run_step_clicked();
    void reset_clicked();
    void fetchData();
public:
    DSFlowControlDockWidget(QWidget* parent, DSAdapter* adapter);
};

#endif // DSFLOWCONTROLDOCKWIDGET_H
