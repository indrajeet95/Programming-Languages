#include "S_Exprsn.h"
map<string,sExp*> sExp::collections;

void sExp::delsATOM(){
    auto it = collections.begin();
    while(it != collections.end()) {
        sExp * symAtom = it->second;
        it->second = NULL;
        delete(symAtom);
        it++;
    }
}

void sExp::isATOM(){
    sExp * expNIL = new sExp(symATOM);
    expNIL->setName("NIL");
    collections["NIL"] = expNIL;
    sExp * expT = new sExp(symATOM);
    expT->setName("T");
    collections["T"] = expT;
}

sExp* sExp::sATOM(string s) {
    if(collections.find(s) == collections.end() || collections[s] == NULL) {
        sExp * ne = new sExp(symATOM);
        ne->setName(s);
        collections[s] = ne;
        return ne;
    }
    else
        return collections[s];
}

sExp::~sExp() {
    if(getType() == notATOM) {
        delete getLeft();
        delete getRight();
    }
}

sExp::sExp(sExpType aType):type(aType){
}
