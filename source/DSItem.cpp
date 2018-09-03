/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSItem.hpp"
#include "DSAdapter.hpp"
#include "QMap"

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
    return fail ? "" : path=" "+path;
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

std::vector<std::string> DSItem::getFeatList() const {
    s_erc error = S_SUCCESS;

    SList* list = SItemFeatKeys(item, &error);
    S_CHK_ERR(&error, S_CONTERR, "getFeatList",
              "Failed to retrieve item feature keys\n");
    std::vector<std::string> std_list(DSAdapter::toStdList(list));

    S_DELETE(list, "getFeatList", &error);
    return std_list;
}

QMap<std::string, std::string> DSItem::getFeatMap() const {
    s_erc error = S_SUCCESS;

    QMap<std::string, std::string> feat_map;

    std::vector<std::string> feat_list(getFeatList());
    for(auto feat_key=feat_list.begin();!(feat_key==(feat_list.end())); ++feat_key) {
        const SObject* o=SItemGetObject(item, feat_key->c_str(), &error);
        if(SObjectIsType(o,"SString",&error)) {
            std::string value(SItemGetString(item, feat_key->c_str(), &error));
            feat_map.insert(*feat_key,value);
        }else
        if(SObjectIsType(o,"SInt",&error)) {
            sint32 value(SItemGetInt(item, feat_key->c_str(), &error));
            feat_map.insert(*feat_key,std::to_string(value));
        }else
        if(SObjectIsType(o,"SFloat",&error)) {
            float value(SItemGetFloat(item, feat_key->c_str(), &error));
            feat_map.insert(*feat_key,std::to_string(value));
        }
    }

    S_CHK_ERR(&error, S_CONTERR, "getFeatMap",
              "Failed to retrieve item feature values\n");
    return feat_map;
}

const SItem* DSItem::getSItem()const {
    if(item!= nullptr)
    return item;
    else
        return nullptr;
}

/*
* Description: check if the item belongs to the given relation
* @param const std::string & - relation
* @return bool
*/
bool DSItem::checkRelation(const std::string&relation) const
{
    s_erc error=S_SUCCESS;
return (relation==SRelationName(SItemRelation(item,&error),&error));
}

/*
* Description: check if the SItem is equal to the SItem found in the given relation at the given Path
* 			   using Speect equals function. Returns:
* 					- 0 if !equal
* 					- 1 if equal and same relation
* 					- 2 if equal and different relations
* @param const std::string& - first item path
* @param const std::string& - second item path
* @return int
*/
int DSItem::IsEqual(const std::string &relation,const std::string& id) const
{
    s_erc error=S_SUCCESS;
    //if relation is equal only check Id otherwise must retrieve the SItem
    bool rel=checkRelation(relation);
    const SItem* item2=
            SItemPathToItem(
                SRelationHead(
                    SUtteranceGetRelation(
                        SItemUtterance(item,&error),relation.c_str(),&error
                        ),&error
                    ),id.c_str(),&error);
    int result=0;
    if(rel)
        result=item==item2?1:0;
    else
        //otherwise relation is different retrieve the item at given path and test Speect equals
        result=SItemEqual(item,item2,&error)?2:0;
    S_DELETE(item2,NULL,&error);
    return result;
}
