#include <iostream>
#include "Examiner.h"
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


sExp * Examiner::evCon(sExp * be, sExp * Alist, sExp * Dlist) {
    if(null(be) == sExp::sATOM("T")) {
        throw std::runtime_error("***error in evcon:: nil condition not valid***");
        return NULL;
    }
    else if(eval(car(car(be)),Alist,&Dlist) != sExp::sATOM("NIL")) {
        if(lengthOfList(car(be)) != 2)
            throw std::runtime_error("***error in evcon:: invalid argument count***");
        return eval(car(cdr(car(be))),Alist,&Dlist);
    }
    else
        return evCon(cdr(be),Alist,Dlist);
}

sExp * Examiner::evLis(sExp * list, sExp * Alist, sExp * Dlist) {
    if(null(list) == sExp::sATOM("T"))
        return sExp::sATOM("NIL");
    else
        return cons(eval(car(list),Alist,&Dlist),evLis(cdr(list),Alist,Dlist));
}

sExp * Examiner::car(sExp * e) {
    if(e->getType() == notATOM)
        return e->getLeft();
    else
        throw std::runtime_error("***error:: car on atomic sExp invalid***");
    return NULL;
}

sExp * Examiner::eq(sExp * e1, sExp * e2) {
    sExp * returnExp = NULL;
    if(e1->getType() == intATOM && e2->getType() == intATOM)
        returnExp = (e1->getVal() == e2->getVal())?  sExp::sATOM("T") :  sExp::sATOM("NIL");
    else if(e1->getType() == symATOM && e2->getType() == symATOM) {
        returnExp = e1->getName() == e2->getName() ?  sExp::sATOM("T"):sExp::sATOM("NIL");
        string ee = returnExp->getName();
    }
    else
        throw std::runtime_error("***error:: incorrect call to eq method***");
    return returnExp;
}

sExp * Examiner::cdr(sExp * e) {
    if(e->getType()== notATOM)
        return e->getRight();
    else
        throw std::runtime_error("***error:: cdr call on primitive sexp***");
    return NULL;
}

sExp * Examiner::cons(sExp * e1, sExp * e2) {
    sExp * newExpression = new sExp(notATOM);
    newExpression->setLeft(e1);
    newExpression->setRight(e2);
    return newExpression;
}

sExp * Examiner::null(sExp * e) {
    return (e == sExp::sATOM("NIL"))? sExp::sATOM("T"):sExp::sATOM("NIL");
}

sExp * Examiner::isInt(sExp * e) {
    return e->getType() == intATOM? sExp::sATOM("T"):sExp::sATOM("NIL");
}

sExp * Examiner::minus(sExp * e1, sExp * e2) {
    if(e1->getType() == intATOM && e2->getType() == intATOM){
        sExp * newExp = new sExp(intATOM);
        newExp->setVal(e1->getVal() - e2->getVal());
        return newExp;
    }
    throw std::runtime_error("***error:: incorrect call to minus method***");
}

sExp * Examiner::plus(sExp * e1, sExp * e2) {
    if(e1->getType() == intATOM && e2->getType() == intATOM) {
        sExp * newExp = new sExp(intATOM);
        newExp->setVal(e1->getVal() + e2->getVal());
        return  newExp;
    }
    throw std::runtime_error("***error:: incorrect call to plus method***");
}

sExp * Examiner::times(sExp * e1,sExp * e2 ) {
    if(e1->getType() == intATOM && e2->getType() == intATOM){
        sExp * newExp = new sExp(intATOM);
        newExp->setVal(e1->getVal() * e2->getVal());
        return newExp;
    }
    throw std::runtime_error("***error:: incorrect call to times method***");
}

sExp * Examiner::quotient(sExp * e1,sExp * e2 ) {
    if(e1->getType() == intATOM && e2->getType() == intATOM){
        sExp * newExp = new sExp(intATOM);
        if(e2->getVal()==0)
            std::runtime_error("***error: divison by 0 invalid***");
        newExp->setVal(e1->getVal()/e2->getVal());
        return  newExp;
    }
    throw std::runtime_error("***error: incorrect call to quotient***");
}

sExp * Examiner::less(sExp * e1,sExp * e2 ) {
    if(e1->getType() == intATOM && e2->getType() == intATOM)
        return e1->getVal()<e2->getVal()? sExp::sATOM("T"):sExp::sATOM("NIL");
    throw std::runtime_error("***error: incorrect call to less***");
}

sExp * Examiner::remainder(sExp * e1, sExp * e2) {
    if(e1->getType() == intATOM && e2->getType() == intATOM) {
        sExp * newExp = new sExp(intATOM);
        if(e2->getVal()==0)
            std::runtime_error("***error:: division by 0 invalid");
        newExp->setVal(e1->getVal() % e2->getVal());
        return newExp;
    }
    throw std::runtime_error("***error: incorrect call to remainder***");
}

sExp * Examiner::greater(sExp * e1,sExp * e2 ) {
    if(e1->getType() == intATOM && e2->getType() == intATOM)
        return e1->getVal()>e2->getVal()? sExp::sATOM("T"):sExp::sATOM("NIL");
    throw std::runtime_error("***error: incorrect call to greater***");
}

sExp * Examiner::atom(sExp * e) {
    return e->getType()!=notATOM? sExp::sATOM("T"): sExp::sATOM("NIL");
}

bool Examiner::isInAlist(sExp * symbolicAtomExp,sExp * aList) {
    if(symbolicAtomExp->getType() != symATOM || aList == sExp::sATOM("NIL"))
        return false;
    if(symbolicAtomExp == car(car(aList)))
        return true;
    else
        return isInAlist(symbolicAtomExp,cdr(aList));
}

bool Examiner::isTrue(sExp * exp) {
    return exp==sExp::sATOM("T");
}

sExp*  Examiner::getValFromAlist(sExp * symbolicAtomExp,sExp * aList) {
    if(symbolicAtomExp == car(car(aList))) return cdr(car(aList));
    else return getValFromAlist(symbolicAtomExp,cdr(aList));
}

sExp* Examiner::addPairsToAList(sExp * parameters, sExp * arguments, sExp * initialAList) {
    if(null(parameters) == sExp::sATOM("T") && null(arguments) == sExp::sATOM("T")) return initialAList;
    else if(null(parameters) != sExp::sATOM("T") && null(arguments) != sExp::sATOM("T")) {
        sExp * newpair = cons(car(parameters),car(arguments));
		initialAList = cons(newpair, initialAList);
		return addPairsToAList(cdr(parameters), cdr(arguments), initialAList);
    }
    else
        throw std::runtime_error("***error:: not matching");
}

sExp * Examiner::addFunctionToDlist(sExp * function, sExp * initialDlist) {
	sExp* functionName = car(car(function));
	sExp* parameterss = car(cdr(car(function)));
	sExp* body = cdr(function);
	return cons(cons(functionName, cons(parameterss, body)), initialDlist);
}

sExp * Examiner::getValFromDlist(sExp* functionName, sExp * dlist) {
    if(isTrue(null(dlist)))
        throw std::runtime_error(">Error:: undeclared function:"+ functionName->getName());
    if(car(car(dlist)) == functionName)
        return cdr(car(dlist));
    else
        return getValFromDlist(functionName,cdr(dlist));
}

void Examiner::assertNumberOfArguments(sExp* funcName,sExp* arguments, int numExpectedArguments) {
    int actualLengthOfArguments = lengthOfList(arguments);
    if( actualLengthOfArguments != numExpectedArguments)
        throw std::runtime_error("***error:: argument count: " + to_string(numExpectedArguments) + " receieve count: " + to_string(actualLengthOfArguments)+" funcition name:"+funcName->getName());
}

int Examiner::lengthOfList(sExp * list) {
    if(list == sExp::sATOM("NIL"))
        return 0;
    else return 1 + lengthOfList(cdr(list));
}

bool Examiner::isList(sExp * exp) {
    if(exp == sExp::sATOM("NIL"))
        return true;
    else if(exp->getType() == notATOM)
        return  isList(exp->getRight());
    else
        return false;
}

sExp * Examiner::eval(sExp * exp, sExp * Alist, sExp ** Dlist) {
    if(exp->getType() != notATOM) {
        if(isInt(exp) == sExp::sATOM("T")) return exp;
        else if(exp == sExp::sATOM("T")) return sExp::sATOM("T");
        else if (null(exp) == sExp::sATOM("T")) return sExp::sATOM("NIL");
        else if(isInAlist(exp,Alist)) return getValFromAlist(exp,Alist);
        else{
            throw std::runtime_error(">Error:: Unbound variable:"+exp->getName());
            return NULL;
        }
    }
    else if (car(exp)->getType() != notATOM) {
        if(eq(car(exp),sExp::sATOM("QUOTE")) == sExp::sATOM("T")) {
            assertNumberOfArguments(car(exp),cdr(exp),1);
            return car(cdr(exp));
        }
        else if(eq(car(exp),sExp::sATOM("COND")) == sExp::sATOM("T")) {
            return evCon(cdr(exp),Alist,*Dlist);
        }
        else if(eq(car(exp),sExp::sATOM("DEFUN")) == sExp::sATOM("T")) {
            sExp * newDlist = addFunctionToDlist(cdr(exp),*Dlist);
            *Dlist = newDlist;
            return sExp::sATOM(car(cdr(exp))->getName());
        }
        else
            return apply(car(exp),evLis(cdr(exp),Alist,*Dlist),Alist,*Dlist);
    }
    else{
        throw std::runtime_error("***error:: unexpected exp in eval***");
        return NULL;
    }
    throw std::runtime_error("***error:: unexpected exp in eval***");
    return NULL;
}

sExp * Examiner::apply(sExp * function,sExp * arguments, sExp * Alist, sExp * Dlist)
{
    sExp * returnVal = NULL;
    if(function->getType() != notATOM) {
        if (eq(function, sExp::sATOM("CAR")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,1);
            returnVal = car(car(arguments));
        } else if (eq(function, sExp::sATOM("CDR")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,1);
            returnVal = cdr(car(arguments));
        } else if (eq(function, sExp::sATOM("CONS")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal =  cons(car(arguments),car(cdr(arguments))); 
        } else if (eq(function, sExp::sATOM("NULL")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,1);
            returnVal = null(car(arguments));
        } else if (eq(function, sExp::sATOM("EQ")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = eq(car(arguments),car(cdr(arguments)));
        } else if (eq(function, sExp::sATOM("ATOM")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,1);
            returnVal = atom(car(arguments));
        } else if (eq(function, sExp::sATOM("INT")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,1);
            returnVal = isInt(car(arguments));
        } else if (eq(function, sExp::sATOM("GREATER")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = greater(car(arguments),car(cdr(arguments)));
        } else if (eq(function, sExp::sATOM("LESS")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = less(car(arguments),car(cdr(arguments)));
        } else if (eq(function, sExp::sATOM("PLUS")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = plus(car(arguments),car(cdr(arguments)));
        } else if (eq(function, sExp::sATOM("MINUS")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = minus(car(arguments),car(cdr(arguments)));
        } else if (eq(function, sExp::sATOM("TIMES")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = times(car(arguments), car(cdr(arguments)));
        } else if (eq(function, sExp::sATOM("QUOTIENT")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = quotient(car(arguments), car(cdr(arguments)));
        }else if (eq(function, sExp::sATOM("REMAINDER")) == sExp::sATOM("T")) {
            assertNumberOfArguments(function,arguments,2);
            returnVal = remainder(car(arguments), car(cdr(arguments)));
        }else
            returnVal = eval(car(cdr(getValFromDlist(function,Dlist))),addPairsToAList(car(getValFromDlist(function,Dlist)),arguments,Alist),&Dlist);
    }
    else {
        throw std::runtime_error("***error:: non atom found in car of apply call***");
        return NULL;
    }
    if(returnVal == NULL) {
        throw std::runtime_error("***error::***");
    }
    return returnVal;
}


