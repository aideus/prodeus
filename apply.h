//
//  apply.h
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__apply__
#define __rand_expr__apply__

// #include <cstdarg>
#include "expression.h"
#include "environment.h"

class Apply: public Expression {
public:
    // can be Lambda or Symbol or anything that evaluates to Lambda
    Apply(const Expression &func, const Expression &argv): Expression(func, argv) {}
    Apply(const Expression &func, const Expression &arg1, const Expression &arg2):
        Expression(func, arg1, arg2) {}
    Apply(const Expression &func, const Expression &arg1,
          const Expression &arg2, const Expression &arg3):
        Expression(func, arg1, arg2, arg3) {}
    Apply(const Expression *func): Expression(func) {}
    virtual Apply* clone() const { return new Apply(*this); }
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual string name() const { return "Apply"; }
};
//typedef Apply A;

Apply Apply0(const Expression &func);

#endif /* defined(__rand_expr__apply__) */
