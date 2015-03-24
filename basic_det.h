//
//  basic_det.h
//  rand_expr
//
//  Created by Alexey Potapov on 01/02/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#ifndef __rand_expr__basic_det__
#define __rand_expr__basic_det__

#include <math.h>
#include "expression.h"

#define MAKE_OP2_CLASS(Name, Op)        \
class Name: public Expression {         \
public:                                 \
    Name(const Expression &child1, const Expression &child2):       \
         Expression(child1, child2) {}                              \
    virtual Name* clone() const { return new Name(*this); }         \
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL) \
        { v = children[0]->getValue() Op children[1]->getValue(); } \
    virtual string name() const { return #Op; }                     \
};

#define MAKE_OP1_CLASS(Name, Op)        \
class Name##_ : public Expression {     \
public:                                 \
    Name##_(const Expression *child1):  \
        Expression(child1) {}           \
    virtual Name##_* clone() const { return new Name##_ (*this); }  \
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL) \
        { v = children[0]->getValue().Op(); }                       \
    virtual string name() const { return #Op; }                     \
};                                      \
Name##_ Name(const Expression& e);



MAKE_OP2_CLASS(Add , +)
MAKE_OP2_CLASS(Sub , -)
MAKE_OP2_CLASS(Mult, *)
MAKE_OP2_CLASS(Div, /)
MAKE_OP2_CLASS(Remainder, %)

MAKE_OP2_CLASS(Ls , < )
MAKE_OP2_CLASS(Lse, <=)
MAKE_OP2_CLASS(Gt , > )
MAKE_OP2_CLASS(Gte, >=)

MAKE_OP1_CLASS(Cos, cos)
MAKE_OP1_CLASS(Sin, sin)
MAKE_OP1_CLASS(Tan, tan)
MAKE_OP1_CLASS(Log, log)
MAKE_OP1_CLASS(Sqrt, sqrt)
MAKE_OP1_CLASS(Exp, exp)
MAKE_OP1_CLASS(Floor, floor)
MAKE_OP1_CLASS(Ceil, ceil)

#endif /* defined(__rand_expr__basic_det__) */
