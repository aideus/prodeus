//
//  lists.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 28/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#include <stdarg.h>
#include "rand_expr.h"


List::List(int num, double v0, ...): Expression()
{
    v = new vector<Data>();
    va_list arguments;
    va_start (arguments, num);
    children.push_back(new Value(v0));
    for(int n = 1; n < num; n++) {
        children.push_back(new Value(va_arg(arguments, double)));
    }
    va_end(arguments);
    // actually, can be evaluated immediately
    // todo? move this functionality to Value (fixed list as value)
}

void List::calcValue(double tp, Expression *pPartner)
{
    vector<Data> *l = new vector<Data>();
    for(int i = 0; i < (int)children.size(); i++) {
        Data vd = children[i]->getValue();
        l->push_back(vd);
    }
    v = l;
}

//todo: bRnd!!
// todo: this functionality can/should be moved to Data
void Cons::calcValue(double tp, Expression *pPartner)
{
    vector<Data> *l = new vector<Data>();
    *l = *(children[0]->getValue().getList());
    l->push_back(children[1]->getValue());
    v = l;  // todo: if v.l is already allocated?.. = should clear allocated list
}

void Cdr_::calcValue(double tp, Expression *pPartner)
{
    vector<Data> *l = new vector<Data>();
    *l = *(children[0]->getValue().getList()); // todo? can be done more efficiently.. is it correct?
    l->pop_back();
    v = l;
    // forceRnd !!
}

void Car_::calcValue(double tp, Expression *pPartner)
{
    // it might seem that it is not necessary to evaluate the list for Car parent,
    // but lambda or whatever can be passed, so evaluation is necessary in general
    // (we need to evaluate children[0] that should be the only child
    //  and if we don't implement lazy evaluations, we have no means to
    //  tell this child to evaluate only the last element)
    v = children[0]->getValue().getList()->back();
    // forceRnd !!
}


void ListRef::calcValue(double tp, Expression *pPartner)
{
    v = (*children[0]->getValue().getList())[children[1]->getValue().getInt()];
    v.forceRnd(v.isRnd() || children[1]->getValue().isRnd());
}

void Nullp_::calcValue(double tp, Expression *pPartner)
{
    Data d = children[0]->getValue();
    v = d.getList()->empty();
    v.forceRnd(d.isRnd());
}

void Length_::calcValue(double tp, Expression *pPartner)
{
    Data d = children[0]->getValue();
    v = (int)d.getList()->size();
    v.forceRnd(d.isRnd());
}

Cdr_ Cdr(const Expression &lst) { return Cdr_(&lst); }

Car_ Car(const Expression &lst) { return Car_(&lst); }

Nullp_ Nullp(const Expression &lst) { return Nullp_(&lst); }

Length_ Length(const Expression &lst) { return Length_(&lst); }

