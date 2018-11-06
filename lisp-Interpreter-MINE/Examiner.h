#include "S_Exprsn.h"
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
    sExp * mul(sExp * e1, sExp * e2);
    sExp * quotient(sExp * e1, sExp * e2);
    sExp * remainder(sExp * e1, sExp * e2);
    sExp * less(sExp * e1, sExp * e2);
    sExp * greater(sExp * e1, sExp * e2);
    sExp*  getValFromAlist(sExp * symbolicAtom, sExp * aList);
    sExp* addpairs(sExp * parameters, sExp * arguments, sExp * initialAList);
    sExp * funcAdder(sExp * function, sExp * initialDlist);
    sExp * dListvaluator(sExp* functionName, sExp * dlist);
    sExp * apply(sExp * function,sExp * arguments, sExp * Alist, sExp * Dlist);
    sExp * evlis(sExp * be, sExp * Alist, sExp * Dlist);
    sExp * evcon(sExp * be, sExp * Alist, sExp * Dlist);
    void argu(sExp* funcName,sExp* arguments, int numExpectedArguments);
    bool isInAlist(sExp * symbolicAtom,sExp * aList);
    bool isList(sExp * exp);
    int listlen(sExp * list);
    bool isTrue(sExp * exp);
public:
    sExp * eval(sExp * exp, sExp * Alist, sExp ** Dlist);
};
