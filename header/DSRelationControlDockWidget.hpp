#ifndef DSRELATIONCONTROLDOCKWIDGET_H
#define DSRELATIONCONTROLDOCKWIDGET_H

#include <QObject>
#include <QDockWidget>
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QQueue>
#include <QString>
#include <QWidget>

#include "DSRelListModel.hpp"

class DSAdapter;

class DSRelationControlDockWidget : public QDockWidget
{
    Q_OBJECT

private:
    DSRelListModel* rel_list_model;
    QListView* list_view;
    QVBoxLayout* v_layout;
    QHBoxLayout* h_layout;
    QPushButton* show_all_button;
    //QPushButton* run_step_button;
    //QPushButton* reset_button;
    //QQueue<QString> proc_queue;

    void setupUI();
    void fillQueue();
    void doConnections();

signals:
    void showSingleRelation();
    void showAllRelations();

public slots:
    void showAllClicked();
    void showSingleClicked();
    void fetchData();
public:
    DSRelationControlDockWidget(QWidget* parent,DSAdapter* adapter);
};

#endif // DSRELATIONCONTROLDOCKWIDGET_H
