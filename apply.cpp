//
//  apply.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include "rand_expr.h"

/*Apply::Apply(ExpressionP func, int num, ...): Expression() {
    children.push_back(func);
    va_list arguments;
    va_start (arguments, num);
    for(int n = 0; n < num; n++) {
        children.push_back(va_arg(arguments, Expression&));
    }
    va_end(arguments);
}*/

ExpressionP Apply::reeval(Environment* pEnv, ExpressionP pPartner, double tp, bool bForceEval)
{
    ExpressionP pEvaluated = shared_from_this();
    if(bForceEval || !bEvaluated || v.isRnd()) {
#ifdef VERBOSE_EVAL
        cout << "Evaluating Apply..." << endl;
#endif
        // lambda will be evaluated to itself (with subtitutions)
        // symbol will be found in env
        ExpressionP newFun = children[0]->reeval(pEnv, getExprChild(pPartner, 0), tp, bForceEval);
        ExpressionConstP lamb = newFun->getValue().getExpression();
        // arguments and inner defines will be added to a new level of env
        pEvaluated = make_shared<Block>();
        pEnv->add_level();
        for(size_t i = 1; i < children.size(); i++) {
            SymbolP s = dynamic_pointer_cast<Symbol>(lamb->getChild(i-1));
            ExpressionP sv = children[i]->reeval(pEnv, getExprChild(pPartner, i),
                                                 tp, bForceEval);
            // todo: if(s == NULL) throw exception
            *pEvaluated << Define(*s, *sv);
            pEnv->push_last(s->getID(), sv);
        }
        // since we put new values into Env, we force reevaluation
        ExpressionP fbody = lamb->getChild(lamb->getChildrenNum()-1)->reeval(pEnv,
            NULL, // TODO ????
            tp, /*bForceEval*/true);
        *pEvaluated << *fbody;
        // pEvaluated->bEvaluated = true;
        // TODO: arguments should be taken only from the upper level of the environment
        // ...TODO: possibly, they should not be placed into the environment, but the closure should
        // be made
        pEvaluated->calcValue();
        pEnv->delete_level();
    }
#ifdef VERBOSE_EVAL
    else { cout << "Skipping: "; }
    cout << "...Apply = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}


