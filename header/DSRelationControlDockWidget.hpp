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

#include<QDialogButtonBox>
#include<QButtonGroup>
#include<QGroupBox>
#include<QCheckBox>

#include<QLabel>

#include "DSRelListModel.hpp"

class DSAdapter;

class DSRelationControlDockWidget : public QDockWidget
{
    Q_OBJECT

private:
    DSRelListModel* rel_list_model;
    QListView* list_view;

    QDialogButtonBox* buttonBox;
    QGroupBox* viewBox;

    QPushButton *showAllButton;

    void setupUI();
    void doConnections();

signals:
    void showRelation(QStringList);

public slots:
    void showAll();
    void updateAvailableRelations();

public:
    DSRelationControlDockWidget(QWidget* parent,DSAdapter* adapter);
};

#endif // DSRELATIONCONTROLDOCKWIDGET_H
