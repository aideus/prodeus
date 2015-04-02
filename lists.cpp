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
    v.set_list_assume_ownership(new vector<Data>());
    va_list arguments;
    va_start (arguments, num);
    children.push_back(make_shared<Value>(v0));
    for(int n = 1; n < num; n++) {
        children.push_back(make_shared<Value>(va_arg(arguments, double)));
    }
    va_end(arguments);
    // actually, can be evaluated immediately
    // todo? move this functionality to Value (fixed list as value)
}

void List::calcValue(double tp, ExpressionP pPartner)
{
    vector<Data> *l = new vector<Data>();
    for(int i = 0; i < (int)children.size(); i++) {
        Data vd = children[i]->getValue();
        l->push_back(vd);
    }
    v.set_list_assume_ownership(l);
}

//todo: bRnd!!
// todo: this functionality can/should be moved to Data
void Cons::calcValue(double tp, ExpressionP pPartner)
{
    vector<Data> *l = new vector<Data>();
    *l = *(children[0]->getValue().getList());
    l->push_back(children[1]->getValue());
    v.set_list_assume_ownership(l);
}

void Cdr_::calcValue(double tp, ExpressionP pPartner)
{
    vector<Data> *l = new vector<Data>();
    *l = *(children[0]->getValue().getList()); // todo? can be done more efficiently.. is it correct?
    l->pop_back();
    v.set_list_assume_ownership(l);
    // forceRnd !!
}

void Car_::calcValue(double tp, ExpressionP pPartner)
{
    // it might seem that it is not necessary to evaluate the list for Car parent,
    // but lambda or whatever can be passed, so evaluation is necessary in general
    // (we need to evaluate children[0] that should be the only child
    //  and if we don't implement lazy evaluations, we have no means to
    //  tell this child to evaluate only the last element)
    v = children[0]->getValue().getList()->back();
    // forceRnd !!
}


void ListRef::calcValue(double tp, ExpressionP pPartner)
{
    v = (*children[0]->getValue().getList())[children[1]->getValue().getInt()];
    v.forceRnd(v.isRnd() || children[1]->getValue().isRnd());
}

void Nullp_::calcValue(double tp, ExpressionP pPartner)
{
    Data d = children[0]->getValue();
    v = d.getList()->empty();
    v.forceRnd(d.isRnd());
}

void Length_::calcValue(double tp, ExpressionP pPartner)
{
    Data d = children[0]->getValue();
    v = (int)d.getList()->size();
    v.forceRnd(d.isRnd());
}



