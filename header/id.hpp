#ifndef ID_H
#define ID_H
#include <QString>

/**
 * @brief models the ID of a node.
 * 				ID is the path to the phisical node from the head of his relation where relation is the relation of the phisical node. 
 * 				The ID is unique within a relation but not among different relations
 */
class ID {
private:
    QString id;
    QString relation;
public:
    
    /**
     * @brief redefinition of the == operator, checks equality between two IDs (never used)
     * @param other ID to be compared
     * @return bool 
     */
    bool operator ==(const ID& other)const;
    
    /**
     * @brief ID constructor
     * @param id identifier
     * @param rel relation
     */
    ID(const QString& id,const QString& rel);
    
    /**
     * @brief redefinition of the = operator, sets assignment
     * @param other ID to be assigned
     * @return ID & 
     */
    ID &operator =(const ID& other);

    /**
     * @brief returns the identification code
     * @return const QString& 
     */
    const QString& getPath()const;
    
    /**
     * @brief returns the relation
     * @return const QString& 
     */
    const QString& getRelation() const;
};

#endif // ID_H
