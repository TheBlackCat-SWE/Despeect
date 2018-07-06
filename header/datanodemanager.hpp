#ifndef DATANODEMANAGER_H
#define DATANODEMANAGER_H
#include <QStandardItemModel>
#include <QStringListModel>
#include <QTableView>
#include "id.hpp"


/**
 * @brief handles the map of data of the node to let the view show it
 */
 
class DataNodeManager:public QObject {
    Q_OBJECT
private:
    QStandardItemModel nodeInfo;
    ID nodeId;
public:
    DataNodeManager();
    /**
     * @brief links the model to a table view
     * @param view
     */
    void linkToModel(QTableView * view);
    
    /**
     * @brief print the node info
     * @param features map node to properties
     * @return void 
     */
    void showNode(const std::map<std::string,std::string>& features);
    
    /**
     * @brief returns the id of the selected node
     * @return const ID
     */
    const ID getNodeId();
public slots:
    /**
     * @brief clears the model
     */
    void clear();
};
#endif // DATANODEMANAGER_H
