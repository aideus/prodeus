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
    List(): Expression() { v = new vector<Data>(); }
    List(int num, double v0, ...);
    List(const Expression &child1, const Expression &child2):
        Expression(child1, child2) {}
    List(const Expression &child1, const Expression &child2, const Expression &child3):
        Expression(child1, child2, child3) {}
    List(const Expression &child1, const Expression &child2, const Expression &child3,
         const Expression &child4): Expression(child1, child2, child3, child4) {}
    // todo... int, Expression, ...
    virtual List* clone() const { return new List(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "List"; }
    ~List() { /*todo?*/ }
};

// cons == push_back; cdr == pop_back
// that's why cons takes lst as the first argument
// not conventional for Lisp, but convinient here
class Cons: public Expression {
public:
    Cons(const Expression &lst, const Expression &val): Expression(lst, val) { }
    virtual Cons* clone() const { return new Cons(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "Cons"; }
    ~Cons() { /*todo?*/ }
};


class ListRef: public Expression {
public:
    ListRef(const Expression &lst, const Expression &val): Expression(lst, val) { }
    virtual ListRef* clone() const { return new ListRef(*this); }  
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "ListRef"; }
    ~ListRef() { /*todo?*/ }
};


class Cdr_: public Expression {
public:
    Cdr_(const Expression *lst): Expression(lst) { }
    virtual Cdr_* clone() const { return new Cdr_(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "Cdr"; }
    ~Cdr_() { /*todo?*/ }
};

Cdr_ Cdr(const Expression &lst);


class Car_: public Expression {
public:
    Car_(const Expression *lst): Expression(lst) { }
    virtual Car_* clone() const { return new Car_(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "Car"; }
    ~Car_() { /*todo?*/ }
};

Car_ Car(const Expression &lst);


class Nullp_: public Expression {
public:
    Nullp_(const Expression *lst): Expression(lst) { }
    virtual Nullp_* clone() const { return new Nullp_(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "Nullp"; }
    ~Nullp_() { /*todo?*/ }
};

Nullp_ Nullp(const Expression &lst);


class Length_: public Expression {
public:
    Length_(const Expression *lst): Expression(lst) { }
    virtual Length_* clone() const { return new Length_(*this); } // todo: list copy?
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL);
    virtual string name() const { return "Length"; }
    ~Length_() { /*todo?*/ }
};

Length_ Length(const Expression &lst);


// TODO: add ListRef, Append, ...

#endif /* defined(__rand_expr__lists__) */
