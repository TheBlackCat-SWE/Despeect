#ifndef PROCESSORMANAGER_H
#define PROCESSORMANAGER_H
#include <QStandardItemModel>
#include <QStandardItem>
#include <QListView>

/**
 * @brief handles the model that allows the printing of the processors
 */
 
class ProcessorManager : public QObject {
    Q_OBJECT
    
private:
    QStandardItemModel* ProcessorModel;
    int indexProcessor;

public:
    ProcessorManager();
    ~ProcessorManager();

    /**
     * @brief links the view to the model
     * @param QListView* v - Qt list view (see Qt docs for more info)
     * @return void
     */
    void linkProcessorModel(QListView* v);
    
    /**
     * @brief sets all the items to the normal font and the last executed processor to none
     * @return void
     */
    void clearLayoutProcessor();
    
    /**
     * @brief locks changes in the model, used while executing
     * @return void
     */
    void lockUpdateItem();
    
    /**
     * @brief unlocks changes in the model
     * @return void
     */
    void unlockUpdateItem();
    
    /**
     * @brief changes the font color to the next processor
     * @return void
     */
    void evidenceNextProcessor();
    
    /**
     * @brief changes the font color of all the processors, if index is valid set the item to bold
     * @return void
     */
    void evidenceAllProcessor();
    
    /**
     * @brief returns whether the index of the processor is valid
     * @return bool
     */
    bool isLayoutClean();
    
    /**
     * @brief returns the list of the selected processor
     * @return std::list<std::string>
     */
    std::list<std::string> getProcessorList();
    
    /**
     * @brief adds a processor to the model
     * @param std::string name - processor name
     * @return void
     */
    void addProcessor(std::string name);
    
    /**
     * @brief clears the model
     * @return void
     */
    void clear();
};

#endif // PROCESSORMANAGER_H
