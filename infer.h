//
//  infer.h
//  rand_expr
//
//  Created by Alexey Potapov on 04/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__infer__
#define __rand_expr__infer__

#include "expression.h"

class AnnealingQuery_: public Expression {
public:
    AnnealingQuery_(const ExpressionP prog):       Expression(prog) {}
    AnnealingQuery_(const Expression& prog, bool): Expression(prog, true) {}
    virtual ExpressionP clone() const { return make_shared<AnnealingQuery_>(*this); }
    virtual string name() const { return "AnnealingQuery"; }
    virtual ExpressionP reeval(Environment* pEnv, ExpressionP pPartner,
                               double tp = 1.0, bool bForceEval = false);

};

class GPQuery_: public Expression {
public:
    GPQuery_(const ExpressionP prog): Expression(prog) {}
    GPQuery_(const Expression& prog, bool): Expression(prog, true) {}
    virtual ExpressionP clone() const { return make_shared<GPQuery_>(*this); }
    virtual string name() const { return "GPQuery"; }
    virtual ExpressionP reeval(Environment* pEnv, ExpressionP pPartner,
                               double tp = 1.0, bool bForceEval = false);
};

AnnealingQuery_ AnnealingQuery(const Expression &prog);
GPQuery_        GPQuery(const Expression &prog);

AnnealingQuery_P sAnnealingQuery(const Expression &prog);
GPQuery_P        sGPQuery(const Expression &prog);



#endif /* defined(__rand_expr__infer__) */
