#include "S_Exprsn.cpp"

class Examiner {
    sExp * car(sExp * e);
    sExp * cdr(sExp * e);
    sExp * cons(sExp * e1, sExp * e2);
    sExp * eq(sExp * e1,sExp * e2);
    sExp * null(sExp * e);
    sExp * isInt(sExp * e);
    sExp * atom(sExp * e);
    sExp * plus(sExp * e1, sExp * e2);
    sExp * minus(sExp * e1, sExp * e2);
    sExp * times(sExp * e1, sExp * e2);
    sExp * quotient(sExp * e1, sExp * e2);
    sExp * remainder(sExp * e1, sExp * e2);
    sExp * less(sExp * e1, sExp * e2);
    sExp * greater(sExp * e1, sExp * e2);
    sExp*  getValFromAlist(sExp * symbolicAtom, sExp * aList);
    sExp* addPairsToAList(sExp * parameters, sExp * arguments, sExp * initialAList);
    sExp * addFunctionToDlist(sExp * function, sExp * initialDlist);
    sExp * getValFromDlist(sExp* functionName, sExp * dlist);
    sExp * apply(sExp * function,sExp * arguments, sExp * Alist, sExp * Dlist);
    sExp * evLis(sExp * be, sExp * Alist, sExp * Dlist);
    sExp * evCon(sExp * be, sExp * Alist, sExp * Dlist);
    void assertNumberOfArguments(sExp* funcName,sExp* arguments, int numExpectedArguments);
    bool isInAlist(sExp * symbolicAtom,sExp * aList);
    bool isList(sExp * exp);
    int lengthOfList(sExp * list);
    bool isTrue(sExp * exp);
public:
    sExp * eval(sExp * exp, sExp * Alist, sExp ** Dlist);
};
