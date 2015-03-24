//
//  quote.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 05/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include "rand_expr.h"

Expression* Quote_::reeval(Environment *pEnv, Expression *pPartner,
                           double tp, bool bForceEval) {
    Quote_ *pEvaluated = this;
    if(bForceEval || !bEvaluated || v.isRnd()) {
        pEvaluated = clone();
        // pEvaluated->children.clear(); // we don't evaluate the child
        pEvaluated->v = children[0];
        // todo? how to set bRnd?
        pEvaluated->bEvaluated = true;
    }
#ifdef VERBOSE_EVAL
    cout << "Quote = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}

Quote_ Quote(const Expression &e) { return Quote_(&e); }


Expression* Eval_::reeval(Environment *pEnv, Expression *pPartner, double tp, bool bForceEval) {
    Eval_ *pEvaluated = this;
    if(bForceEval || !bEvaluated || v.isRnd()) {
        pEvaluated = clone();
        pEvaluated->children.clear();
        Expression *child = children[0]->reeval(pEnv, getExprChild(pPartner, 0), tp, bForceEval);
        pEvaluated->children.push_back(child);
        pEvaluated->v = const_cast<Expression *>
            (child->getValue().getExpression())->reeval(pEnv,
                                                        NULL, // TODO?????
                                                        tp, bForceEval)->getValue();
        // the problem is - if the quoted expression is itself random
        // its evaluation trace will be lost...
        pEvaluated->bEvaluated = true;
    }
#ifdef VERBOSE_EVAL
    cout << "Eval = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}

Eval_ Eval(const Expression &e) { return Eval_(&e); }


