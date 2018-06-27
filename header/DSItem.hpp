/**
  * @author Riccardo Damiani
  * @version 1.1 26/05/18 (Additions: getFeatList and getFeatMap)
  */

#ifndef DSITEM_H
#define DSITEM_H

#include "speect.h"
#include <string>
#include <vector>
#include <map>


/**
 * @brief The DSItem class
 * Wrapper class that gives access to the static functionalities found in
 * the @c SItem speect object. It represents a node of the structure graph
 * inside the Heterogeneous Relation Graph.
 */
class DSItem {
private:
    const SItem* item;
    DSItem(const SItem* item);
public:
    static DSItem* create(const SItem* item);

    const bool Equals(const DSItem& other) const{
        s_erc error = S_SUCCESS;
        return SItemEqual(item, other.item, &error);
    }

    /**
     * @brief getName
     * Gets the given item's name.
     * @return The given item's name.
     */
    std::string getName() const;

    /*
     * TODO
     */
    std::string getPath() const;
    /*
     * TODO
     */
    std::string getRelation() const;

    std::string getId() const {
        return getRelation()+":"+getPath();
    }
    /**
     * @brief next
     * Gets the item next to the given one in the current relation.
     * @return Pointer to the next item.
     * @note The caller is responsible for the memory of the returned item???
     */
    DSItem* next() const;
    /**
     * @brief next
     * Gets the item previous to the given one in the current relation.
     * @return Pointer to the previous item.
     * @note The caller is responsible for the memory of the returned item???
     */
    DSItem* previous() const;
    /**
     * @brief parent
     * Returns the parent item of the given item.
     * @return Pointer to the parent item of the given item.
     * @note The caller is responsible for the memory of the returned item???
     */
    DSItem* parent() const;
    /**
     * @brief child
     * Retrieves the first child item of the given item.
     * @return Pointer to the first child item of the given item.
     * @note The caller is responsible for the memory of the returned item???
     */
    DSItem* child() const;
    /**
     * @brief child
     * Retrieves the @c nth child item of the given item.
     * @param n The index of the daughter to return.
     * @note Indexing starts at 0.
     * @return Pointer to the @c nth child item of the given item.
     * @note The caller is responsible for the memory of the returned item???
     */
    DSItem* child(int n) const;
    /**
     * @brief lastChild
     * Retrieves the last child item of the given item.
     * @return Pointer to the last child item of the given item.
     * @note The caller is responsible for the memory of the returned item???
     */
    DSItem* lastChild() const;
    /**
     * @brief getFeatList
     * Gets the names of the features of the given item.
     * @return A list containing the feature keys
     */
    std::vector<std::string> getFeatList() const;
    /**
     * @brief getFeatMap
     * Retrieves all the keys and values for the current item.
     * @return A map containing all the names associated with their
     * corresponding values.
     */
    std::map<std::string, std::string> getFeatMap() const;
    // TODO Grant access through paths

};

#endif // DSITEM_H
