//
//  environment.h
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__environment__
#define __rand_expr__environment__

#include <vector>
#include <map> // FIX map -> unordered_map
#include "expression.h"

using namespace std;

typedef string Index;

class Environment {
public:
    Environment() {
        // top-level environment is added immediately
        map <Index, Expression*> um;
        es.push_back(um);
    }
    Expression *find_symbol(Index id) {
        for(int i = (int)es.size()-1; i >= 0; i--) {
            map<Index, Expression*>::const_iterator got = es[i].find(id);
            if(got != es[i].end()) return got->second;
        }
        // todo?
        return NULL;
    }
    void add_level() {
        map <Index, Expression*> um;
        es.push_back(um);
    }
    void delete_level() {
        es.pop_back();
    }
    void push_last(Index id, Expression *pVal) {
        // todo: check if exists
        es.back()[id] = pVal;
    }
protected:
    vector<map <Index, Expression*>> es; // vector <>
};

#endif /* defined(__rand_expr__environment__) */
