//
//  let.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 01/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include <stdarg.h>
#include "rand_expr.h"

ExpressionP Define::reeval(Environment *pEnv, ExpressionP pPartner, double tp, bool bForceEval)
{
    DefineP pEvaluated = static_pointer_cast<Define>(shared_from_this());
    if(bForceEval || !bEvaluated || v.isRnd()) {
#ifdef VERBOSE_EVAL
        cout << "Evaluating Define..." << endl;
#endif
        pEvaluated = static_pointer_cast<Define>(clone());
        // symbol in definition is not changed
        // actually, one would like to extend semantics to accept Define(If(Flip(), x, y), Value(...))
        pEvaluated->children.pop_back(); // popping old definition body
        ExpressionP pEvBody = children[1]->reeval(pEnv, getExprChild(pPartner, 1), tp, bForceEval);
        pEvaluated->children.push_back(pEvBody);
        pEvaluated->bEvaluated = false; // definitions should be always reevaluated since they dynamically change environment?
        pEnv->push_last(dynamic_pointer_cast<Symbol>(children[0])->getID(), pEvBody);
    }
#ifdef VERBOSE_EVAL
    else { cout << "Skipping: "; }
    cout << "...Define = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}

ExpressionP Define::clone_rec_and_substitute(Environment *pEnv, vector<SymbolP> &args) const
{
    DefineP pClone = static_pointer_cast<Define>(clone());
    pClone->children.clear();
    // parsing symbol being defined (should not be substituted)
    args.push_back(dynamic_pointer_cast<Symbol>(children[0]));
    children[0]->forceEvaluated(false);
    pClone->children.push_back(children[0]->clone());
    // cloning child (body definition)
    ExpressionP pCh = children.back()->clone_rec_and_substitute(pEnv, args);
    pClone->v.forceRnd(v.isRnd() || pCh->getValue().isRnd());
    pClone->children.push_back(pCh);
    pClone->bEvaluated = false;
    return pClone;
}

ExpressionP Block::reeval(Environment *pEnv, ExpressionP pPartner, double tp, bool bForceEval)
{
    pEnv->add_level();
    ExpressionP pEvaluated = Expression::reeval(pEnv, pPartner, tp, bForceEval);
    pEnv->delete_level();
    return pEvaluated;
}

void Block::calcValue(double tp, ExpressionP pPartner)
{
    v = children.back()->getValue();
}
