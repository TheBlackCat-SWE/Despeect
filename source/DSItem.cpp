/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSItem.hpp"
#include "DSAdapter.hpp"

DSItem::DSItem(const SItem *item) : item(item) {}

DSItem* DSItem::create(const SItem* item) {
    if(item != nullptr)
        return new DSItem(item);
    else
        return nullptr;
}

std::string DSItem::getName() const {
    s_erc error=S_SUCCESS;
    if(SItemFeatureIsPresent(item,"name",&error))
        return SItemGetName(item,&error);
    else
        return "";
}


std::string DSItem::getPath() const {
    s_erc error = S_SUCCESS;
    const SItem * it = const_cast<const SItem *>(item);
    std::string path="";
    const SRelation* rel=SItemRelation(it,&error);
    bool fail=false;
    SItem *temp=NULL;
    while(!SItemEqual(it,SRelationHead(rel,&error),&error)&&!fail)
    {
        temp=SItemPrev(it,&error);
        if(temp!=NULL)
        {
            path=".n"+path;
            it=temp;
        }
        else{
            temp=SItemParent(it,&error);
            if(temp!=NULL)
            {
                path=".daughter"+path;
                it=temp;
            }
            else
                fail=true;
        }
    }
    return fail?"":path=" "+path;
}

std::string DSItem::getRelation() const {
    s_erc error = S_SUCCESS;
    const SRelation* rel=SItemRelation(item,&error);
    std::string relName = std::string(SRelationName(rel,&error));
    return relName;
}

DSItem *DSItem::next() const {
    s_erc error = S_SUCCESS;
    const SItem* next = SItemNext(item, &error);

    S_CHK_ERR(&error, S_CONTERR, "next",
              "Failed to get next item\n");
    return create(next);
}

DSItem *DSItem::previous() const {
    s_erc error = S_SUCCESS;

    const SItem* previous = SItemPrev(item, &error);
    S_CHK_ERR(&error, S_CONTERR, "previous",
              "Failed to get previous item\n");
    return create(previous);
}

DSItem *DSItem::parent() const {
    s_erc error = S_SUCCESS;

    const SItem* parent = SItemParent(item, &error);
    S_CHK_ERR(&error, S_CONTERR, "parent",
              "Failed to get parent item\n");
    return create(parent);

}

DSItem *DSItem::child() const {
    s_erc error = S_SUCCESS;

    const SItem* child = SItemDaughter(item, &error);
    S_CHK_ERR(&error, S_CONTERR, "child",
              "Failed to get child item\n");
    return create(child);
}

DSItem *DSItem::child(int n) const {
    s_erc error = S_SUCCESS;

    const SItem* child = SItemNthDaughter(item, n, &error);
    S_CHK_ERR(&error, S_CONTERR, "child",
              "Failed to get the no. %d child item\n", n);
    return create(child);
}

DSItem *DSItem::lastChild() const {
    s_erc error = S_SUCCESS;

    const SItem* child = SItemLastDaughter(item, &error);
    S_CHK_ERR(&error, S_CONTERR, "lastChild",
              "Failed to get last child item\n");
   return create(child);
}

std::vector<std::__cxx11::string> DSItem::getFeatList() const {
    s_erc error = S_SUCCESS;

    SList* list = SItemFeatKeys(item, &error);
    S_CHK_ERR(&error, S_CONTERR, "getFeatList",
              "Failed to retrieve item feature keys\n");
    std::vector<std::string> std_list(DSAdapter::toStdList(list));

    S_DELETE(list, "getFeatList", &error);
    return std_list;
}

std::map<std::string, std::string> DSItem::getFeatMap() const {
    s_erc error = S_SUCCESS;

    std::map<std::string, std::string> feat_map;

    std::vector<std::string> feat_list(getFeatList());
    for(auto&& feat_key : feat_list) {
        feat_map[feat_key] = SItemGetString(item, feat_key.c_str(), &error);
    }

    S_CHK_ERR(&error, S_CONTERR, "getFeatMap",
              "Failed to retrieve item feature values\n");
    return feat_map;
}
