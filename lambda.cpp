//
//  lambda.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 01/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include "rand_expr.h"

ExpressionP Lambda::reeval(Environment *pEnv, ExpressionP pPartner,
                           double tp, bool bForceEval)
{
    LambdaP pEvaluated = static_pointer_cast<Lambda>(shared_from_this());
    if(bForceEval || !bEvaluated || v.isRnd()) {
#ifdef VERBOSE_EVAL
        cout << "Evaluating Lambda..." << endl;
#endif
        // all children except the last are symbols evaluated by Apply
        // .. partial application?
        // lambda is evaluated to itself
        // TODO: actually, should be evaluated to closure
        // TODO: or alternatively, we can (recursively) replace all symbols in *this with their values
        // from the environment
        pEvaluated = static_pointer_cast<Lambda>(clone());
        vector<SymbolP > args;
        pEvaluated->v = clone_rec_and_substitute(pEnv, args);
        pEvaluated->v.forceRnd(pEvaluated->v.getExpression()->isRnd()); // randomness is passed via expression... 
        // todo: forceRnd()... are random substitutions made
        pEvaluated->bEvaluated = true;
    }
#ifdef VERBOSE_EVAL
    else { cout << "Skipping: "; }
    cout << "...Lambda = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}

ExpressionP Lambda::clone_rec_and_substitute(Environment *pEnv, vector<SymbolP> &args) const
{
    LambdaP pClone = static_pointer_cast<Lambda>(clone());
    pClone->children.clear();
    // parsing formal arguments
    for(int i = 0; i < (int)children.size()-1; i++) {
        args.push_back(dynamic_pointer_cast<Symbol>(children[i]));
        children[i]->forceEvaluated(false);
        pClone->children.push_back(children[i]->clone());
    }
    ExpressionP pCh = children.back()->clone_rec_and_substitute(pEnv, args);
    pClone->v.forceRnd(v.isRnd() || pCh->getValue().isRnd());
    pClone->children.push_back(pCh);
    pClone->bEvaluated = false;
    return pClone;
}

Lambda Lambda0(const Expression &body) { return Lambda(body, true); }

