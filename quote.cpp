//
//  quote.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 05/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include "rand_expr.h"

ExpressionP Quote_::reeval(Environment *pEnv, ExpressionP pPartner,
                           double tp, bool bForceEval) {
    Quote_P pEvaluated = static_pointer_cast<Quote_>(shared_from_this());
    if(bForceEval || !bEvaluated || v.isRnd()) {
        pEvaluated = static_pointer_cast<Quote_>(clone());
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



ExpressionP Eval_::reeval(Environment *pEnv, ExpressionP pPartner, double tp, bool bForceEval) {
    Eval_P pEvaluated = static_pointer_cast<Eval_>(shared_from_this());
    if(bForceEval || !bEvaluated || v.isRnd()) {
        pEvaluated = static_pointer_cast<Eval_>(clone());
        pEvaluated->children.clear();
        ExpressionP child = children[0]->reeval(pEnv, getExprChild(pPartner, 0), tp, bForceEval);
        pEvaluated->children.push_back(child);
        pEvaluated->v = 
            (child->getValue().getExpression())->reeval(pEnv,
                                                        ExpressionP(), // TODO?????
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



