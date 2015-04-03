//
//  expression.h
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva, Sergey Rodionov All rights reserved.
//

#ifndef __rand_expr__expression__
#define __rand_expr__expression__

#include <vector>
#include <sstream>
#include "data.h"
#include "pointers.h"

using namespace std;

class Environment;
class Ls;
class Gt;
class Lse;
class Gte;
class Add;
class Sub;
class Mult;
class Div;
class Apply;
class Remainder;
class ListRef;


class Expression: public enable_shared_from_this<Expression>
{
public:
    virtual ~Expression(){};
    Expression() {
        // v.bDef = false;
        // todo: check if default constructor for data is called
        bEvaluated = false; // false by default
    }
    Expression(const ExpressionConstP child) {
       children.push_back(child->clone());
       bEvaluated = false;
    }
    Expression(const Expression& child, bool) {
       children.push_back(child.clone());
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
    virtual ExpressionP clone() const {
        return make_shared<Expression>(*this);
    }
    // deep clone - cloning all children, but without allocated data
    ExpressionP clone_rec() const;
    // recursively cloning and trying to substitute all symbols except formal arguments
    ExpressionP clone_rec_and_substitute(Environment* pEnv, vector<SymbolP> &args) const;
    const bool isRnd() const { return v.isRnd(); }
    const bool isChildrenRnd() const;
    virtual ExpressionP reeval(Environment* pEnv, ExpressionP pPartner,
                               double tp = 1.0, bool bForceEval = false);
    virtual bool shallow_equal(ExpressionP pExpr);
    virtual ExpressionP eval();
    // calculating value without evaluating children; supplementary for reeval
    // can be made protected, since should be called only from eval
    // by default pPartner is empty pointer (equivalent of NULL)
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP()) {}
    virtual string name() const { return "Expression"; }
    void forceEvaluated(bool bEv) { bEvaluated = bEv;}
    ExpressionP getChild(int i) const { return children.at(i); }
    const int getChildrenNum() const { return (int)children.size(); }
    const Data& getValue() const { return v; }
    virtual const double getInstantLogp() { return 0.; }
    const double getRecLogp();
    Expression& operator<<(const Expression &child) {
        children.push_back(child.clone());
        return *this;
    }
    Expression& operator<<(const ExpressionConstP child) {
        return operator<<(*child);
    }
    virtual string to_s();

    // syntactic sugar   
    //we have extra copy here, but it is ok if you use it only during initialization
    virtual Ls  operator<(const Expression &expr);
    virtual Ls  operator<(int n);
    virtual Ls  operator<(double d);
    virtual Gt  operator>(const Expression &expr);
    virtual Gt  operator>(int n);
    virtual Gt  operator>(double d);
    virtual Lse operator<=(const Expression &expr);
    virtual Lse operator<=(int n);
    virtual Lse operator<=(double d);
    virtual Gte operator>=(const Expression &expr);
    virtual Gte operator>=(int n);
    virtual Gte operator>=(double d);
    // todo: ==
    
    virtual Add  operator+(const Expression &expr);
    virtual Add  operator+(int n);
    virtual Add  operator+(double d);
    virtual Sub  operator-(const Expression &expr);
    virtual Sub  operator-(int n);
    virtual Sub  operator-(double d);
    virtual Mult operator*(const Expression &expr);
    virtual Mult operator*(int n);
    virtual Mult operator*(double d);
    virtual Div  operator/(const Expression &expr);
    virtual Div  operator/(int n);
    virtual Div  operator/(double d);
    virtual Remainder operator%(const Expression &expr);
    virtual Remainder operator%(int n);
    virtual Remainder operator%(double d);
    virtual Apply     operator()(const Expression &arg1, const Expression &arg2,
                                   const Expression &arg3);
    virtual Apply     operator()(const Expression &arg1, const Expression &arg2);
    virtual Apply     operator()(const Expression &expr);
    virtual Apply     operator()(double d);
    virtual Apply     operator()();
    virtual ListRef   operator[](int n);
    virtual ListRef   operator[](const Expression &n);
protected:
    bool bEvaluated;
    Data v;
    vector<ExpressionP> children;
};


typedef vector<ExpressionP> ExpVec;
typedef vector<ExpressionP>::const_iterator ExpCIter;
typedef vector<ExpressionP>::iterator ExpIter;


//this function can return empty pointer if pExpr is empty pointer
ExpressionP getExprChild(ExpressionP pExpr, int i);




#endif /* defined(__rand_expr__expression__) */
