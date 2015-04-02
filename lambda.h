//
//  lambda.h
//  rand_expr
//
//  Created by Alexey Potapov on 01/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__lambda__
#define __rand_expr__lambda__

#include "expression.h"
#include "symbol.h"

class Lambda: public Expression {
public:
    Lambda(const ExpressionP body):       Expression(body) { }
    Lambda(const Expression& body, bool): Expression(body, true) { }
    Lambda(const Symbol &arg1, const Expression &body): Expression(arg1, body) { }
    Lambda(const Symbol &arg1, const Symbol &arg2, const Expression &body):
        Expression(arg1, arg2, body) { }
    Lambda(const Symbol &arg1, const Symbol &arg2,
           const Symbol &arg3, const Expression &body):
        Expression(arg1, arg2, arg3, body) { }
    ExpressionP clone_rec_and_substitute(Environment* pEnv, vector<SymbolP> &args) const;
    virtual ExpressionP reeval(Environment *pEnv, ExpressionP pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual ExpressionP clone() const { return make_shared<Lambda>(*this); }
    virtual string name() const { return "Lambda"; }
};

Lambda Lambda0(const Expression &body);

#endif /* defined(__rand_expr__lambda__) */
