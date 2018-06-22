/**
  * @author Riccardo Damiani
  * @version 1.0 21/06/18
  */

#ifndef DSTREEDOCKWIDGET_H
#define DSTREEDOCKWIDGET_H

#include "DSTreeModel.hpp"
#include "DSAdapter.hpp"
#include "DSTreeItem.hpp"
#include <QDockWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QQueue>
#include <QString>
#include <QWidget>
#include <vector>
#include <string>


class DSTreeDockWidget: public QDockWidget {
private:
    DSAdapter* adapter;
    DSTreeModel* tree_model;
    QTreeView* tree_view;
    QVBoxLayout* v_layout;
    QHBoxLayout* h_layout;
    QPushButton* run_all_button;
    QPushButton* run_step_button;
    QPushButton* reset_button;
    QQueue<QString> proc_queue;

    void setupUI() {
        tree_view->setModel(tree_model);

        v_layout->addWidget(tree_view);
        v_layout->addLayout(h_layout);
        h_layout->addWidget(run_all_button);
        h_layout->addWidget(run_step_button);
        h_layout->addWidget(reset_button);

        QWidget* internal_widget = new QWidget();
        internal_widget->setLayout(v_layout);
        setWidget(internal_widget);
        setWindowTitle("Flow Control");
    }
    void fillQueue() {
        //FIXME Vedi sotto..
        /*
         * Meglio usare una coda di QPersistentModelIndex, per evitare successivamente di
         * non riuscire a distinguere tra utt_proc omonimi ma appartenenti ad utt_type
         * diversi, volendo selezionare quello giusto in run_step
         */
        reset_clicked();
        QModelIndex root = tree_view->rootIndex();
        if(root.isValid()) {
            for(int i = 0; tree_model->rowCount(root); i++) {
                QModelIndex utt_type = tree_model->index(i, 0, root);
                for(int j = 0; tree_model->rowCount(utt_type); j++) {
                    QModelIndex utt_proc = tree_model->index(i, 0, utt_type);
                    DSTreeItem* item = static_cast<DSTreeItem*>(utt_proc.internalPointer());
                    if(item->isChecked())
                        proc_queue.enqueue(item->data(utt_proc.column()).toString());
                }
            }
        }
    }
public slots:
    void run_all_clicked() {
        fillQueue();
        std::vector<std::string> proc_list;
        while(!proc_queue.isEmpty())
            proc_list.push_back(proc_queue.dequeue().toStdString());
        adapter->execUttProcList(proc_list);
        //TODO Set arrow icon (and select) on last processor inside the view
    }
    void run_step_clicked() {
        if(proc_queue.isEmpty()) fillQueue();
        adapter->execUttProc(proc_queue.dequeue().toStdString());
        //TODO Set arrow icon (and select) on last processor inside the view
    }
    void reset_clicked() {
        proc_queue.clear();
        adapter->resetUtterance();
        //TODO Remove arrow icon (and select) from last processor inside the view
    }
    void fetchData() { tree_model->fetchData(); }
public:
    DSTreeDockWidget(QWidget* parent, DSAdapter* adapter):
        QDockWidget(parent),
        adapter(adapter),
        tree_model(new DSTreeModel(this, adapter)),
        tree_view(new QTreeView(this)),
        v_layout(new QVBoxLayout),
        h_layout(new QHBoxLayout),
        run_all_button(new QPushButton("Run All")),
        run_step_button(new QPushButton("Run Step")),
        reset_button(new QPushButton("Reset"))
    {
        setupUI();
    }
};

#endif // DSTREEDOCKWIDGET_H
