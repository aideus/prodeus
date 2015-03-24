//
//  symbol.h
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__symbol__
#define __rand_expr__symbol__

#include "expression.h"
#include "environment.h"

#ifdef WIN32
extern _ULonglong g_index;
#else
extern long int g_index;
#endif

class Symbol: public Expression {
public:
    Symbol(string s): Expression() { id = s; }
    Symbol(): Expression() {
        id = "__var__" + to_string(g_index);
        g_index++;
    }
    virtual Symbol* clone() const { return new Symbol(*this); }
    virtual Expression* reeval(Environment *pEnv, Expression *pPartner,
                               double tp = 1.0, bool bForceEval = false);
    // virtual void calcValue(double tp = 1.0) {}; // cannot separately calculate value since it need environment
    Index getID() { return id; }
    virtual string name() const { return "S:" + id; }
protected:
    Index id;
};

typedef Symbol S;

#endif /* defined(__rand_expr__symbol__) */
