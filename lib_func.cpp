//
//  lib_func.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 05/02/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#include "rand_expr.h"

Symbol repeat("repeat"), maplist("map"), foldr("foldr");


namespace libfunc_imp {

    // internal symbols for simlicity
    Symbol _f("_f"), _n("_n"), _lst("_lst"), _x("_x");

    Define repeat_def(repeat,
                      Lambda(_n, _f,
                             If(_n < 1, List(),
                                Cons(repeat(_n-1, _f), _f()))));

    Define map_def(maplist,
                   Lambda(_f, _lst,
                          If(Nullp(_lst), List(),
                             Cons(maplist(_f, Cdr(_lst)), _f(Car(_lst))))));
    
    Define foldr_def(foldr,
                     Lambda(_f, _lst, _x,
                            If(Nullp(_lst), _x,
                               _f(Car(_lst), foldr(_f, Cdr(_lst), _x)))));

}


Expression &include_libdefs() {
    Expression *pe = new Expression();
    *pe << libfunc_imp::repeat_def
        << libfunc_imp::map_def
        << libfunc_imp::foldr_def;
    
    return *pe;
}

