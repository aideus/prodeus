//
//  expression.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//


//#include <typeinfo>
#include "rand_expr.h"

Expression::~Expression()
{
    // ...
}

const bool Expression::isChildrenRnd() const
{
    bool bR = false;
    for(ExpCIter it = children.begin(); it != children.end(); ++it)
        bR = bR || (*it)->isRnd();
    return bR;
}

Expression *Expression::reeval(Environment *pEnv, Expression *pPartner, double tp, bool bForceEval)
{
    if(pPartner != NULL && !shallow_equal(pPartner)) {
        // it is probably possible to choose the parent randomly, but they are already random,
        // so we prefer the first parent for consistency
        pPartner = NULL;
    }
    Expression *pEvaluated = this;
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

Expression *Expression::eval()
{
    Environment *pEnv = new Environment();
    Expression *p = reeval(pEnv, NULL, 1.0, true);
    delete pEnv;
    return p;
}

bool Expression::shallow_equal(Expression *pExpr)
{
    return name() == pExpr->name() && children.size() == pExpr->children.size();
    // REM: implement more detailed comparison for Value, Symbol, ?Apply
}


Expression* Expression::clone_rec() const {
    Expression *pClone = clone();
    pClone->children.clear();
    for(ExpCIter it = children.begin(); it != children.end(); ++it)
        pClone->children.push_back((*it)->clone_rec());
    return pClone;
}

Expression* Expression::clone_rec_and_substitute(Environment *pEnv, vector<Symbol *> &args) const
{
    bool bRnd = v.isRnd();
    Expression *pClone = clone();
    pClone->children.clear();
    for(ExpCIter it = children.begin(); it != children.end(); ++it) {
        Symbol *s = dynamic_cast<Symbol *>(*it);
        bool bReplace = true;
        if(s != NULL) {
            for(int i = 0; i < (int)args.size(); i++) {
                if(s->getID() == args[i]->getID()) {
                    bReplace = false;
                    break;
                }
            }
            if(bReplace) {
                Expression *pSubs = pEnv->find_symbol(s->getID());
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
    for(vector<Expression *>::iterator it = children.begin(); it != children.end(); ++it)
        s += (*it)->to_s() + " ";
    if(!children.empty()) s += ")";
    s += "=" + v.to_s();
    return s;
}


// ================== Syntactic sugar operators ==================

Expression& Expression::operator<(const Expression &expr) {
    return *(new Ls(*this, expr));
}
Expression& Expression::operator<(int n) { return *this < V(n); }
Expression& Expression::operator<(double d) { return *this < V(d); }

Expression& Expression::operator>(const Expression &expr) {
    return *(new Gt(*this, expr));
}
Expression& Expression::operator>(int n) { return *this > V(n); }
Expression& Expression::operator>(double d) { return *this > V(d); }

Expression& Expression::operator<=(const Expression &expr) {
    return *(new Lse(*this, expr));
}
Expression& Expression::operator<=(int n) { return *this <= V(n); }
Expression& Expression::operator<=(double d) { return *this <= V(d); }

Expression& Expression::operator>=(const Expression &expr) {
    return *(new Gte(*this, expr));
}
Expression& Expression::operator>=(int n) { return *this >= V(n); }
Expression& Expression::operator>=(double d) { return *this >= V(d); }



Expression& Expression::operator+(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return *(new Add(*this, expr));
}
Expression& Expression::operator+(int n) { return *this + V(n); }
Expression& Expression::operator+(double d) { return *this + V(d); }


Expression& Expression::operator-(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return *(new Sub(*this, expr));
}
Expression& Expression::operator-(int n) { return *this - V(n); }
Expression& Expression::operator-(double d) { return *this - V(d); }


Expression& Expression::operator*(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return *(new Mult(*this, expr));
}
Expression& Expression::operator*(int n) { return *this * V(n); }
Expression& Expression::operator*(double d) { return *this * V(d); }

Expression& Expression::operator/(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return *(new Div(*this, expr));
}
Expression& Expression::operator/(int n) { return *this / V(n); }
Expression& Expression::operator/(double d) { return *this / V(d); }

Expression& Expression::operator%(const Expression &expr) {
    //  TODO? Values can be processed more efficiently
    return *(new Remainder(*this, expr));
}
Expression& Expression::operator%(int n) { return *this % V(n); }
Expression& Expression::operator%(double d) { return *this % V(d); }


Expression& Expression::operator()(const Expression &arg1, const Expression &arg2,
                                   const Expression &arg3) {
    return *(new Apply(*this, arg1, arg2, arg3));
}

Expression& Expression::operator()(const Expression &arg1, const Expression &arg2) {
    return *(new Apply(*this, arg1, arg2));
}

Expression& Expression::operator()(const Expression &expr) {
    return *(new Apply(*this, expr));
}

Expression& Expression::operator()(double d) {
    return *(new Apply(*this, V(d)));
}

Expression& Expression::operator()() {
    return *(new Apply(this));
}

// todo: can be not only for Lists... generalize?
Expression& Expression::operator[](int n)
{
    return *(new ListRef(*this, V(n)));
}

Expression& Expression::operator[](const Expression &n)
{
    return *(new ListRef(*this, n));
}



Expression *getExprChild(Expression *pExpr, int i)
{
    return pExpr == NULL ? NULL : pExpr->getChild(i);
}
