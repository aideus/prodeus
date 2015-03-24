//
//  conditions.h
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__conditions__
#define __rand_expr__conditions__

#include "expression.h"

// if-then-else
//TODO: else is not necessary
class If: public Expression {
public:
    If(const Expression &cnd, const Expression &thn, const Expression &els):
       Expression(cnd, thn, els) {}
    virtual If* clone() const { return new If(*this); }
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "If"; }
};

#endif /* defined(__rand_expr__conditions__) */
