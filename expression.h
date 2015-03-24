//
//  expression.h
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#ifndef __rand_expr__expression__
#define __rand_expr__expression__

#include <vector>
#include <sstream>
#include "data.h"

using namespace std;

class Environment;

class Expression {
public:
    virtual ~Expression();
    Expression() {
        // v.bDef = false;
        // todo: check if default constructor for data is called
        bEvaluated = false; // false by default
    }
    Expression(const Expression* child) {
        *this << *child;
        bEvaluated = false;
    }
    Expression(const Expression &child1, const Expression &child2) {
        children.push_back(child1.clone());
        children.push_back(child2.clone());
        bEvaluated = false;
    }
    Expression(const Expression &child1, const Expression &child2, const Expression &child3) {
        children.push_back(child1.clone());
        children.push_back(child2.clone());
        children.push_back(child3.clone());
        bEvaluated = false;
    }
    Expression(const Expression &child1, const Expression &child2,
               const Expression &child3, const Expression &child4) {
        children.push_back(child1.clone());
        children.push_back(child2.clone());
        children.push_back(child3.clone());
        children.push_back(child4.clone());
        bEvaluated = false;
    }
    Expression(const Expression &child1, const Expression &child2, const Expression &child3,
               const Expression &child4, const Expression &child5) {
        children.push_back(child1.clone());
        children.push_back(child2.clone());
        children.push_back(child3.clone());
        children.push_back(child4.clone());
        children.push_back(child5.clone());
        bEvaluated = false;
    }
    Expression(const Expression &child1, const Expression &child2, const Expression &child3,
               const Expression &child4, const Expression &child5, const Expression &child6) {
        children.push_back(child1.clone());
        children.push_back(child2.clone());
        children.push_back(child3.clone());
        children.push_back(child4.clone());
        children.push_back(child5.clone());
        children.push_back(child6.clone());
        bEvaluated = false;
    }
    // shallow clone
    // !! allocated value data and children are not copied
    virtual Expression* clone() const {
        return new Expression(*this);
        //Expression *pe = new Expression(); *pe = *this; return pe;
    }
    // deep clone - cloning all children, but without allocated data
    Expression* clone_rec() const;
    // recursively cloning and trying to substitute all symbols except formal arguments
    Expression* clone_rec_and_substitute(Environment *pEnv, vector<class Symbol *> &args) const;
    const bool isRnd() const { return v.isRnd(); }
    const bool isChildrenRnd() const;
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual bool shallow_equal(Expression *pExpr);
    virtual Expression *eval();
    // calculating value without evaluating children; supplementary for reeval
    // can be made protected, since should be called only from eval
    virtual void calcValue(double tp = 1.0, Expression *pPartner = NULL) {}
    virtual string name() const { return "Expression"; }
    void forceEvaluated(bool bEv) { bEvaluated = bEv;}
    Expression* getChild(int i) const { return children[i]; }
    const int getChildrenNum() const { return (int)children.size(); }
    const Data getValue() const { return v; }
    virtual const double getInstantLogp() { return 0.; }
    const double getRecLogp();
    Expression& operator<<(const Expression &child) {
        children.push_back(child.clone());
        return *this;
    }
    virtual string to_s();

    // syntactic sugar
    virtual Expression& operator<(const Expression &expr);
    virtual Expression& operator<(int n);
    virtual Expression& operator<(double d);
    virtual Expression& operator>(const Expression &expr);
    virtual Expression& operator>(int n);
    virtual Expression& operator>(double d);
    virtual Expression& operator<=(const Expression &expr);
    virtual Expression& operator<=(int n);
    virtual Expression& operator<=(double d);
    virtual Expression& operator>=(const Expression &expr);
    virtual Expression& operator>=(int n);
    virtual Expression& operator>=(double d);
    // todo: ==
    
    virtual Expression& operator+(const Expression &expr);
    virtual Expression& operator+(int n);
    virtual Expression& operator+(double d);
    virtual Expression& operator-(const Expression &expr);
    virtual Expression& operator-(int n);
    virtual Expression& operator-(double d);
    virtual Expression& operator*(const Expression &expr);
    virtual Expression& operator*(int n);
    virtual Expression& operator*(double d);
	virtual Expression& operator/(const Expression &expr);
    virtual Expression& operator/(int n);
    virtual Expression& operator/(double d);
	virtual Expression& operator%(const Expression &expr);
    virtual Expression& operator%(int n);
    virtual Expression& operator%(double d);
    virtual Expression& operator()(const Expression &arg1, const Expression &arg2,
                                   const Expression &arg3);
    virtual Expression& operator()(const Expression &arg1, const Expression &arg2);
    virtual Expression& operator()(const Expression &expr);
    virtual Expression& operator()(double d);
    virtual Expression& operator()();
    virtual Expression& operator[](int n);
    virtual Expression& operator[](const Expression &n);
protected:
    bool bEvaluated;
    Data v;
    vector<Expression *> children;
};


typedef vector<Expression *> ExpVec;
typedef vector<Expression *>::const_iterator ExpCIter;
typedef vector<Expression *>::iterator ExpIter;

Expression *getExprChild(Expression *pExpr, int i);

#endif /* defined(__rand_expr__expression__) */
