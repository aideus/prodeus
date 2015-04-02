//
//  lists.h
//  rand_expr
//
//  Created by Alexey Potapov on 28/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#ifndef __rand_expr__lists__
#define __rand_expr__lists__

#include "expression.h"
#include "value.h"

//TODO: efficient list management - no unnecessary copying, memory (de)allocation...

class List: public Expression {
public:
    List(): Expression() { v.set_list_assume_ownership(new vector<Data>()); }
    List(int num, double v0, ...);
    List(const Expression &child1, const Expression &child2):
        Expression(child1, child2) {}
    List(const Expression &child1, const Expression &child2, const Expression &child3):
        Expression(child1, child2, child3) {}
    List(const Expression &child1, const Expression &child2, const Expression &child3,
         const Expression &child4): Expression(child1, child2, child3, child4) {}
    // todo... int, Expression, ...
    virtual ExpressionP clone() const { return make_shared<List>(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "List"; }
};

// cons == push_back; cdr == pop_back
// that's why cons takes lst as the first argument
// not conventional for Lisp, but convinient here
class Cons: public Expression {
public:
    Cons(const Expression &lst, const Expression &val): Expression(lst, val) { }
    virtual ExpressionP clone() const { return make_shared<Cons>(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "Cons"; }
};

class ListRef: public Expression {
public:
    ListRef(const Expression &lst, const Expression &val): Expression(lst, val) { }
    virtual ExpressionP clone() const { return make_shared<ListRef>(*this); }  
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "ListRef"; }
};


class Cdr_: public Expression {
public:
    Cdr_(const ExpressionConstP lst): Expression(lst) { }
    Cdr_(const Expression& lst, bool): Expression(lst, true) { }
    virtual ExpressionP clone() const { return make_shared<Cdr_>(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "Cdr"; }
};

inline Cdr_ Cdr(const Expression &lst) { return Cdr_(lst, true); }


class Car_: public Expression {
public:
    Car_(const ExpressionConstP lst): Expression(lst) { }
    Car_(const Expression& lst, bool): Expression(lst, true) { }
    virtual ExpressionP clone() const { return make_shared<Car_>(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "Car"; }
    ~Car_() { /*todo?*/ }
};

inline Car_ Car(const Expression &lst) { return Car_(lst, true); }


class Nullp_: public Expression {
public:
    Nullp_(const ExpressionConstP lst): Expression(lst) { }
    Nullp_(const Expression& lst, bool): Expression(lst, true) { }
    virtual ExpressionP clone() const { return make_shared<Nullp_>(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "Nullp"; }
};

inline Nullp_ Nullp(const Expression &lst) { return Nullp_(lst, true); }


class Length_: public Expression {
public:
    Length_(const ExpressionConstP lst): Expression(lst) { }
    Length_(const Expression& lst, bool): Expression(lst, true) { }
    virtual ExpressionP clone() const { return make_shared<Length_>(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "Length"; }
};

inline Length_ Length(const Expression &lst) {return Length_(lst, true);}


// TODO: add ListRef, Append, ...

#endif /* defined(__rand_expr__lists__) */
