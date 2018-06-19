/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSRelation.hpp"
#include "DSItem.hpp"


std::__cxx11::string DSRelation::getName() const {
    s_erc error = S_SUCCESS;

    const char* name = SRelationName(rel, &error);
    S_CHK_ERR(&error, S_CONTERR, "getName",
              "Failed to retrieve relation name\n");

    return std::string(name);
}

DSItem *DSRelation::getHead() const {
    s_erc error = S_SUCCESS;

    const SItem* head = SRelationHead(rel, &error);
    S_CHK_ERR(&error, S_CONTERR, "getHead",
              "Failed to obtain head item\n");
    return DSItem::create(head);
}

DSItem *DSRelation::getTail() const {
    s_erc error = S_SUCCESS;

    const SItem* tail = SRelationTail(rel, &error);
    S_CHK_ERR(&error, S_CONTERR, "getTail",
              "Failed to obtain tail item\n");
    return DSItem::create(tail);
}
