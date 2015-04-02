//
//  basic_rnd.h
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__basic_rnd__
#define __rand_expr__basic_rnd__

#include "expression.h"
#include "value.h"

class Flip_: public Expression
{
public:
    Flip_(double p_): Expression() { p = p_; }
    Flip_(const ExpressionConstP e): Expression()  { *this << *e; p = -1.; }
    Flip_(const Expression& e, bool): Expression() { *this <<  e; p = -1.; }
    virtual ExpressionP clone() const { return make_shared<Flip_>(*this); }
   
    // by defual ExpressionP is an empty pointer
    virtual void calcValue(double tp = 1.0, ExpressionP = ExpressionP());
    virtual const double getInstantLogp() {
		// Flip can be presented e.g. in function definition, where it is not evaluated
		return bEvaluated ? logps : 0.0;
	}
    virtual string name() const { return "Flip"; }

private:
    double logps;
    double p;
};



inline Flip_ Flip()                    { return Flip_(0.5); }
inline Flip_ Flip(double p)            { return Flip_(p); }
inline Flip_ Flip(const Expression &e) { return Flip_(e, true); }


class Gaussian: public Expression
{
public:
    Gaussian(double x0, double sigma): Expression() {
        // for the fixed params, we can, actually, process them more efficiently...
        children.push_back(make_shared<Value>(x0));
        children.push_back(make_shared<Value>(sigma));
    }
    Gaussian(const Expression &x0, const Expression &sigma): Expression(x0, sigma) { }
    virtual ExpressionP clone() const { return make_shared<Gaussian>(*this); }
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual const double getInstantLogp() {
		// Gaussian can be presented e.g. in function definition, where it is not evaluated
		return bEvaluated ? logps : 0.0;
	}
    virtual string name() const { return "Gaussian"; }

private:
    double logps;
};

class RndInt_: public Expression
{
public:
    RndInt_(const ExpressionConstP e): Expression()       { *this << *e; }
    RndInt_(const Expression& e, bool): Expression()      { *this <<  e; }
    virtual ExpressionP clone() const { return make_shared<RndInt_>(*this); }
    virtual void calcValue(double tp = 1.0, ExpressionP = ExpressionP());
    virtual const double getInstantLogp() {
		// RndInt can be presented e.g. in function definition, where it is not evaluated
		return bEvaluated ? logps : 0.0;
	}
    virtual string name() const { return "RndInt"; }

private:
    double logps;
};


RndInt_ RndInt(int n);
RndInt_ RndInt(const Expression &e);


#ifdef SUPPORT_OPENCV
class MatRndInt: public Expression
{
public:
    MatRndInt(int rows, int cols, int type, int n): Expression() {
        children.push_back(make_shared<Value>(rows));
        children.push_back(make_shared<Value>(cols));
        children.push_back(make_shared<Value>(type));
        children.push_back(make_shared<Value>(n));
    }
    MatRndInt(int rows, int cols, int type, int n, const cv::Mat &im0): Expression() {
        children.push_back(make_shared<Value>(rows));
        children.push_back(make_shared<Value>(cols));
        children.push_back(make_shared<Value>(type));
        children.push_back(make_shared<Value>(n));
        children.push_back(make_shared<Value>(im0));
    }
    MatRndInt(const Expression &rows, const Expression &cols,
              const Expression &type, const Expression &n): Expression(rows, cols, type, n) { }
    MatRndInt(const Expression &rows, const Expression &cols, const Expression &type,
              const Expression &n, const Expression &im0): Expression(rows, cols, type, n, im0) { }
    virtual ExpressionP clone() const {
        MatRndIntP pClone = make_shared<MatRndInt>(*this);
        if(v.getMat() != NULL)
            pClone->v = *v.getMat(); // copying data
        return pClone;
    }
    virtual void calcValue(double tp = 1.0, ExpressionP = ExpressionP());
    virtual const double getInstantLogp() { return bEvaluated ? logps : 0.0; }
    virtual string name() const { return "MatRndInt"; }
    
private:
    double logps;
};

class MatGaussian: public Expression
{
public:
    MatGaussian(int rows, int cols, int type, double x0, double sigma): Expression() {
        children.push_back(make_shared<Value>(rows));
        children.push_back(make_shared<Value>(cols));
        children.push_back(make_shared<Value>(type));
        children.push_back(make_shared<Value>(x0));
        children.push_back(make_shared<Value>(sigma));
    }
    MatGaussian(int rows, int cols, int type, const cv::Mat &x0, double sigma): Expression() {
        children.push_back(make_shared<Value>(rows));
        children.push_back(make_shared<Value>(cols));
        children.push_back(make_shared<Value>(type));
        children.push_back(make_shared<Value>(x0));
        children.push_back(make_shared<Value>(sigma));
    }
    MatGaussian(const Expression &rows, const Expression &cols, const Expression &type,
              const Expression &x0, const Expression &sigma): Expression(rows, cols, type, x0, sigma) { }
   virtual ExpressionP clone() const {
        MatGaussianP pClone = make_shared<MatGaussian>(*this);
        if(v.getMat() != NULL)
            pClone->v = *v.getMat(); // copying data
        return pClone;
    }
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual const double getInstantLogp() { return bEvaluated ? logps : 0.0; }
    virtual string name() const { return "MatRndInt"; }
    
private:
    double logps;
};

#endif

#endif /* defined(__rand_expr__basic_rnd__) */

