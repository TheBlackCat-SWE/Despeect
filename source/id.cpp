#include "id.hpp"


const QString& ID::getRelation() const {
    return relation;
}


bool ID::operator ==(const ID &other) const {
    return id==other.id&&relation==other.relation;
}


ID::ID(const QString& id,const QString& rel):id(id),relation(rel){
}


ID &ID::operator =(const ID &other) {
    if(this!=&other)
    {
        this->id=QString(other.id);
        this->relation=QString(other.relation);
    }
    return *this;
}


const QString& ID::getPath()const {
    return id;
}


