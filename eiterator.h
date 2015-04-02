//
//  eiterator.h
//  rand_expr
//
//  Created by Alexey Potapov on 28/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__eiterator__
#define __rand_expr__eiterator__

#include "expression.h"

// todo: this iterator will work correctly only for trees, not graphs

class EIterator {
public:
    enum Order {
        parent_first = 0,
        child_first = 1
    };
    EIterator(ExpressionP root, Order o_): o(o_) {
        //switch(o)
        current = root;
    }
    //EIterator& operator++() {
    EIterator& next() {
        switch(o) {
            case parent_first:
                if(current->getChildrenNum() != 0) {
                    stack.push_back(current);
                    indices.push_back(0);
                    current = current->getChild(0);
                    return *this;
                }
                while(stack.size() != 0) {
                    ExpressionP parent = stack.back();
                    int idx = indices.back();
                    if(idx < parent->getChildrenNum() - 1) {
                        current = parent->getChild(idx + 1);
                        indices.back() = idx + 1;
                        return *this;
                    }
                    stack.pop_back();
                    indices.pop_back();
                    current = parent;
                }
                current = NULL;
                return *this;
            case child_first:
                // todo
                return *this;
        }
    }
    ExpressionP operator*() { return current; }
    
private:
    vector<ExpressionP> stack;
    vector<int> indices;
    ExpressionP current;
    Order o;
};


#endif /* defined(__rand_expr__eiterator__) */
