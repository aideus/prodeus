//
//  value.h
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//



#ifndef __rand_expr__value__
#define __rand_expr__value__

#include "expression.h"

class Value: public Expression {
public:
    Value(): Expression() { }
    Value(double d): Expression() {
        bEvaluated = true;
        v = d;
    }
    Value(int n): Expression() {
        bEvaluated = true;
        v = n;
    }
    Value(Data d): Expression() {
        bEvaluated = true;
        v = d;
    }
#ifdef SUPPORT_OPENCV
    Value(cv::Mat *m): Expression() {
        bEvaluated = true;
        v = *m;
    }
    Value(const cv::Mat &m): Expression() {
        bEvaluated = true;
        v = m;
    }
#endif
    virtual ExpressionP reeval(Environment *pEnv, ExpressionP pPartner,
                               double tp = 1.0, bool bForceEval = false)
        { return shared_from_this(); /* do nothing */ } //todo? Verbose; cloning?
    // virtual void calcValue(double tp = 1.0) {}; // nothing to calculate
    virtual ExpressionP clone() const { return make_shared<Value>(*this); }
    virtual string name() const { return "V"; }
};
typedef Value V;

#endif /* defined(__rand_expr__value__) */
