//
//  conditions.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 27/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include "rand_expr.h"

Expression* If::reeval(Environment *pEnv, Expression *pPartner, double tp, bool bForceEval)
{
    If *pEvaluated = this;
    if(bForceEval || !bEvaluated || v.isRnd()) {
#ifdef VERBOSE_EVAL
        cout << "Evaluating If..." << endl;
#endif
        pEvaluated = clone();
        pEvaluated->children.clear();
        Expression *pCond = children[0]->reeval(pEnv, getExprChild(pPartner, 0), tp, bForceEval);
        pEvaluated->children.push_back(pCond);
        if(pCond->getValue().getInt()) {
            pEvaluated->children.push_back(children[1]->reeval(pEnv,
                getExprChild(pPartner, 1), tp, bForceEval));
            // TODO? check if pPartner->children[2] is evaluated...
            pEvaluated->children.push_back(children[2]);
        } else {
            // TODO? check if pPartner->children[1] is evaluated...
            pEvaluated->children.push_back(children[1]);
            pEvaluated->children.push_back(children[2]->reeval(pEnv,
                getExprChild(pPartner, 2), tp, bForceEval));
        }
        pEvaluated->calcValue(tp, pPartner);
        bEvaluated = true;
    }
#ifdef VERBOSE_EVAL
    else { cout << "Skipping: "; }
    cout << "...If = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}

void If::calcValue(double tp, Expression *pPartner)
{
    if(children[0]->getValue().getInt()) {
        v = children[1]->getValue();
    } else {
        v = children[2]->getValue();
    }
    bool bRnd = v.isRnd() || children[0]->getValue().isRnd();
    v.forceRnd(bRnd);
}
