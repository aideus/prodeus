//
//  quote.h
//  rand_expr
//
//  Created by Alexey Potapov on 05/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__quote__
#define __rand_expr__quote__

#include "expression.h"

class Quote_: public Expression {
public:
    Quote_(const Expression *e): Expression(e) { }
    virtual Quote_* clone() const { return new Quote_(*this); }
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual string name() const { return "Quote"; }

};

Quote_ Quote(const Expression &e);


class Eval_: public Expression {
public:
    Eval_(const Expression *e): Expression(e) { }
    virtual Eval_* clone() const { return new Eval_(*this); }
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual string name() const { return "Eval"; }
    
};

Eval_ Eval(const Expression &e);

#endif /* defined(__rand_expr__quote__) */
