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
    Quote_(const ExpressionP e): Expression(e) { }
    Quote_(const Expression& e, bool): Expression(e, true) { }
    virtual ExpressionP clone() const { return make_shared<Quote_>(*this); }
    virtual ExpressionP reeval(Environment *pEnv, ExpressionP pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual string name() const { return "Quote"; }

};

inline Quote_ Quote(const Expression &e) { return Quote_(e, true); }

class Eval_: public Expression {
public:
    Eval_(const ExpressionConstP e): Expression(e) { }
    Eval_(const Expression& e, bool): Expression(e, true) { }
    virtual ExpressionP clone() const { return make_shared<Eval_>(*this); }
    virtual ExpressionP reeval(Environment *pEnv, ExpressionP pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual string name() const { return "Eval"; }
    
};

inline Eval_ Eval(const Expression &e) { return Eval_(e, true); }

#endif /* defined(__rand_expr__quote__) */
