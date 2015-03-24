//
//  symbol.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include <iostream>
#include "rand_expr.h"

using namespace std;

#ifdef WIN32
_ULonglong g_index = 1000;
#else
long int g_index = 1000;
#endif


Expression *Symbol::reeval(Environment *pEnv, Expression *pPartner, double tp, bool bForceEval)
{
    Symbol *pEvaluated = clone();
    pEvaluated->children.clear(); // actually, 'this' shouldn't have children
    if(bForceEval || !bEvaluated || v.isRnd()) {
        // shouldn't have children; thus, we don't evaluate them, don't call Expression::reeval
        Expression *pVal = pEnv->find_symbol(id);
        // pVal is evaluated during definition
        // (even if it is a function, we use its value (evaluation is not application)
        // TODO: check if it is found
        pEvaluated->v = pVal->getValue();
        // is it correct to make (clone of) the symbol evaluated?
        // it cannot be addressed in the program being evaluated,
        // but can it be addressed in a different context in future runs?
        // ... something like (if p (define x a) (define x b)) - it is not correct in lisp, but...
        pEvaluated->bEvaluated = true;
    } else pEvaluated = this;
#ifdef VERBOSE_EVAL
    cout << "Symbol addressed: " << id << " = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}
