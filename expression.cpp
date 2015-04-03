//
//  expression.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva, Sergey Rodionov All rights reserved.
//


//#include <typeinfo>
#include "rand_expr.h"
#include "environment.h"

const bool Expression::isChildrenRnd() const
{
    bool bR = false;
    for(ExpCIter it = children.begin(); it != children.end(); ++it)
        bR = bR || (*it)->isRnd();
    return bR;
}

ExpressionP Expression::reeval(Environment* pEnv, ExpressionP pPartner, double tp, bool bForceEval)
{
    if(pPartner && !shallow_equal(pPartner)) {
        // it is probably possible to choose the parent randomly, but they are already random,
        // so we prefer the first parent for consistency
        pPartner = ExpressionP(); //create empty pointer (equivalent of NULL)
    }
    ExpressionP pEvaluated = shared_from_this();
    if(bForceEval || !bEvaluated || v.isRnd()) {
#ifdef VERBOSE_EVAL
        cout << "Evaluating " << name() << "..." << endl;
#endif
        pEvaluated = clone();
        pEvaluated->children.clear();
        for(size_t i = 0; i < children.size(); i++) {
            pEvaluated->children.push_back(children[i]->reeval(pEnv,
                getExprChild(pPartner, i), tp, bForceEval));
        }
        pEvaluated->calcValue(tp, pPartner);
        pEvaluated->bEvaluated = true;
    }
#ifdef VERBOSE_EVAL
    else cout << "Skipped: ";
    cout << "...Evaluated: " << name();
    if(pEvaluated->children.size() > 0) {
        cout << "(" << pEvaluated->children[0]->getValue();
        if(pEvaluated->children.size() > 1)
            cout << ", " << pEvaluated->children[1]->getValue();
        cout << ")";
    }
    cout << " = " << pEvaluated->getValue() << endl; // to return full copy is more convenient?
#endif
    return pEvaluated;
}

ExpressionP Expression::eval()
{
    Environment Env;
    return reeval(&Env, ExpressionP(), 1.0, true);
}

bool Expression::shallow_equal(ExpressionP pExpr)
{
    return name() == pExpr->name() && children.size() == pExpr->children.size();
    // REM: implement more detailed comparison for Value, Symbol, ?Apply
}


ExpressionP Expression::clone_rec() const {
    ExpressionP pClone = clone();
    pClone->children.clear();
    for(ExpCIter it = children.begin(); it != children.end(); ++it)
        pClone->children.push_back((*it)->clone_rec());
    return pClone;
}

ExpressionP Expression::clone_rec_and_substitute(Environment* pEnv, vector<SymbolP> &args) const
{
    bool bRnd = v.isRnd();
    ExpressionP pClone = clone();
    pClone->children.clear();
    for(ExpCIter it = children.begin(); it != children.end(); ++it) {
        SymbolP s = dynamic_pointer_cast<Symbol>(*it);
        bool bReplace = true;
        if(s != NULL) {
            for(int i = 0; i < (int)args.size(); i++) {
                if(s->getID() == args[i]->getID()) {
                    bReplace = false;
                    break;
                }
            }
            if(bReplace) {
                ExpressionP pSubs = pEnv->find_symbol(s->getID());
                if(pSubs != NULL) {
                    //pClone->children.push_back(pSubs->clone_rec_and_substitute(pEnv, args));
                    *pClone << Block(Define(*s, *pSubs), *s);
                    bRnd = bRnd || pSubs->getValue().isRnd();
                }
                else bReplace = false; // symbol can be absent in the current context
            }
        } else bReplace = false;
        // todo? inherit randomness from other children also?
        if(!bReplace)
            pClone->children.push_back((*it)->clone_rec_and_substitute(pEnv, args));
    }
    pClone->bEvaluated = false; // clone is not evaluated
    pClone->v.forceRnd(bRnd); // using v to pass if substitution is random
    return pClone;
}


// todo? this will work correctly only for trees, not graphs
const double Expression::getRecLogp()
{
    double sum = getInstantLogp();
    for(ExpCIter it = children.begin(); it != children.end(); ++it) {
        sum += (*it)->getRecLogp();
    }
    return sum;
}

string Expression::to_s()
{
    string s = name();
    /*const void *address = static_cast<const void*>(this);
     std::stringstream ss;
     ss << address;
     s = "[" + ss.str() + "]";*/
    if(!children.empty()) s += "(";
    for(vector<ExpressionP>::iterator it = children.begin(); it != children.end(); ++it)
        s += (*it)->to_s() + " ";
    if(!children.empty()) s += ")";
    s += "=" + v.to_s();
    return s;
}


// ================== Syntactic sugar operators ==================

Ls Expression::operator<(const Expression &expr) {
    return Ls(*this, expr);
}
Ls Expression::operator<(int n) { return *this < V(n); }
Ls Expression::operator<(double d) { return *this < V(d); }

Gt Expression::operator>(const Expression &expr) {
    return Gt(*this, expr);
}
Gt Expression::operator>(int n) { return *this > V(n); }
Gt Expression::operator>(double d) { return *this > V(d); }

Lse Expression::operator<=(const Expression &expr) {
    return Lse(*this, expr);
}
Lse Expression::operator<=(int n) { return *this <= V(n); }
Lse Expression::operator<=(double d) { return *this <= V(d); }

Gte Expression::operator>=(const Expression &expr) {
    return Gte(*this, expr);
}
Gte Expression::operator>=(int n) { return *this >= V(n); }
Gte Expression::operator>=(double d) { return *this >= V(d); }



Add Expression::operator+(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return Add(*this, expr);
}
Add Expression::operator+(int n) { return *this + V(n); }
Add Expression::operator+(double d) { return *this + V(d); }


Sub Expression::operator-(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return Sub(*this, expr);
}
Sub Expression::operator-(int n) { return *this - V(n); }
Sub Expression::operator-(double d) { return *this - V(d); }


Mult Expression::operator*(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return Mult(*this, expr);
}
Mult Expression::operator*(int n) { return *this * V(n); }
Mult Expression::operator*(double d) { return *this * V(d); }

Div Expression::operator/(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return Div(*this, expr);
}
Div Expression::operator/(int n) { return *this / V(n); }
Div Expression::operator/(double d) { return *this / V(d); }

Remainder Expression::operator%(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return Remainder(*this, expr);
}
Remainder Expression::operator%(int n) { return *this % V(n); }
Remainder Expression::operator%(double d) { return *this % V(d); }


Apply Expression::operator()(const Expression &arg1, const Expression &arg2,
                                   const Expression &arg3) {
    return Apply(*this, arg1, arg2, arg3);
}

Apply Expression::operator()(const Expression &arg1, const Expression &arg2) {
    return Apply(*this, arg1, arg2);
}

Apply Expression::operator()(const Expression &expr) {
    return Apply(*this, expr);
}

Apply Expression::operator()(double d) {
    return Apply(*this, V(d));
}

Apply Expression::operator()() {
    return Apply(*this, true);
}

// todo: can be not only for Lists... generalize?
ListRef Expression::operator[](int n)
{
    return ListRef(*this, V(n));
}

ListRef Expression::operator[](const Expression &n)
{
    return ListRef(*this, n);
}



ExpressionP getExprChild(ExpressionP pExpr, int i)
{
   return pExpr ? pExpr->getChild(i) : ExpressionP();
}
