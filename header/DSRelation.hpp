/**
  * @author Riccardo Damiani
  * @version 1.0 26/05/18
  */

#ifndef DSRELATION_H
#define DSRELATION_H

#include "speect.h"
#include <string>

class DSItem;


/**
 * @brief The DSRelation class
 * Wrapper class that gives access to the static functionalities found in
 * the @c SRelation speect object. It represents a structure graph inside the
 * Heterogeneous Relation Graph.
 */
class DSRelation {
private:
    const SRelation* rel;
public:
    DSRelation(const SRelation* rel) : rel(rel) {}
    /**
     * @brief getName
     * Gets the name of given relation.
     * @return The @c std::string containing the name of the relation.
     */
    std::string getName() const;
    /**
     * @brief getHead
     * Return the item at the head of the given relation.
     * @return A @c DSItem wrapper of the speect's @c SItem for the head
     * item of the relation.
     * @note The caller is responsible for the memory of the returned obj
     */
    DSItem* getHead() const;
    /**
     * @brief getTail
     * Return the item at the tail of the given relation.
     * @return A @c DSItem wrapper of the speect's @c SItem for the tail
     * item of the relation.
     * @note The caller is responsible for the memory of the returned obj
     */
    DSItem* getTail() const;
};

#endif // DSRELATION_H
