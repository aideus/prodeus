//
//  let.h
//  rand_expr
//
//  Created by Alexey Potapov on 01/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__let__
#define __rand_expr__let__

#include "expression.h"
#include "symbol.h"

class Define: public Expression {
public:
    Define(const Symbol &child1, const Expression &child2): Expression(child1, child2) {}
    Define(const string &child1, const Expression &child2): Expression(Symbol(child1), child2) {}
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual Define* clone() const { return new Define(*this); }
    Expression* clone_rec_and_substitute(Environment *pEnv, vector<class Symbol *> &args) const;
    virtual string name() const { return "Define"; }
};

// almost as Expression, but the value of the last child is taken as the result
// can be used to construct something like Let, but arbitrary usage is possible
class Block: public Expression {
public:
    Block(): Expression() {}
    Block(const Expression &child1, const Expression &child2): Expression(child1, child2) {}
    Block(const Expression &child1, const Expression &child2, const Expression &child3):
        Expression(child1, child2, child3) {}
    virtual Block* clone() const { return new Block(*this); }
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "Block"; }
};


#endif /* defined(__rand_expr__let__) */
