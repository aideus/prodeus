//
//  data.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#include <math.h>
#include <string>
#include "data.h"
#include "expression.h"

using namespace std;

// todo: check if it is needed to be copied

Data::~Data()
{
    /*todo*/
    // if(bDef)
    // if(t == cvmat_t) { delete data.pmat; }
}

Data& Data::operator=(Expression *pExpr)
{
    data.pe = pExpr;
    t = expr_t;
    bDef = true;
    return *this;
}

Data& Data::operator=(vector<Data> *pL)
{
    data.l = pL;
    bDef = true;
    bRnd = false;
    for(vector<Data>::iterator it = pL->begin(); it != pL->end(); ++it)
        bRnd = bRnd || it->bRnd;
    t = list_t;
    return *this;
}

#ifdef SUPPORT_OPENCV
Data& Data::operator=(cv::Mat *pMat)
{
    data.pmat = new cv::Mat(pMat->clone()); // Data should be copied
    t = cvmat_t;
    bDef = true;
    return *this;
}

Data& Data::operator=(const cv::Mat &mat)
{
    data.pmat = new cv::Mat(mat.clone()); // Data should be copied
    t = cvmat_t;
    bDef = true;
    return *this;
}
#endif

// TODO
#ifdef SUPPORT_OPENCV
#define IMPLEMENT_OPERATION(Op)                     \
Data Data::operator Op(const Data &r) const {       \
    Data v;                                         \
    /* TODO? double dispatch */                     \
    if(t == double_t) {                             \
        if(r.t == double_t) v = data.d Op r.data.d; \
        if(r.t == int_t) v = data.d Op r.data.n;    \
        if(r.t == cvmat_t) v = data.d Op *r.data.pmat;     \
    }                                               \
    if(t == int_t) {                                \
        if(r.t == double_t) v = data.n Op r.data.d; \
        if(r.t == int_t) v = data.n Op r.data.n;    \
        if(r.t == cvmat_t) v = data.n Op *r.data.pmat;     \
    }                                               \
    if(t == cvmat_t) {                              \
        if(r.t == double_t) v = *data.pmat Op r.data.d;    \
        if(r.t == int_t) v = *data.pmat Op r.data.n;       \
        if(r.t == cvmat_t) v = *data.pmat Op *r.data.pmat;  /*check sizes?*/ \
    }                                               \
    /* TODO? expression */                          \
    /* TODO? exception */                           \
    v.bRnd = bRnd || r.bRnd;                        \
    return v;                                       \
}
#else
#define IMPLEMENT_OPERATION(Op)                     \
Data Data::operator Op(const Data &r) const {       \
    Data v;                                         \
    /* TODO? double dispatch */                     \
    if(t == double_t) {                             \
    if(r.t == double_t) v = data.d Op r.data.d; \
        if(r.t == int_t) v = data.d Op r.data.n;    \
    }                                               \
    if(t == int_t) {                                \
        if(r.t == double_t) v = data.n Op r.data.d; \
        if(r.t == int_t) v = data.n Op r.data.n;    \
    }                                               \
    /* TODO? expression */                          \
    /* TODO? exception */                           \
    v.bRnd = bRnd || r.bRnd;                        \
    return v;                                       \
}
#endif


Data Data::operator %(const Data &r) const {
    Data v;
    /* TODO? double dispatch */
    if(t == (int)double_t) {
        if(r.t == (int)double_t) v = (int)data.d % (int)r.data.d;
        if(r.t == int_t) v = (int)data.d % r.data.n;
    }
    if(t == int_t) {
        if(r.t == (int)double_t) v = data.n % (int)r.data.d;
        if(r.t == int_t) v = data.n % r.data.n;
    }
    /* TODO? expression */
    /* TODO? exception */
    v.bRnd = bRnd || r.bRnd;
    return v;
}

IMPLEMENT_OPERATION(+)
IMPLEMENT_OPERATION(-)
IMPLEMENT_OPERATION(*)
IMPLEMENT_OPERATION(/)

IMPLEMENT_OPERATION(<)
IMPLEMENT_OPERATION(<=)
IMPLEMENT_OPERATION(>)
IMPLEMENT_OPERATION(>=)


// TODO
#ifdef SUPPORT_OPENCV
#define IMPLEMENT_CVFUNC(Op) \
  case cvmat_t: \
  break;
#else
#define IMPLEMENT_CVFUNC(Op)
#endif

#define IMPLEMENT_FUNCTION(Op)                  \
Data Data::Op() const {                         \
    Data v;                                     \
    switch(t) {                                 \
        case double_t: v = ::Op(data.d); break;       \
        case int_t: v = ::Op((double)data.n); break;  \
        IMPLEMENT_CVFUNC(Op)                          \
        default: break;/*todo*/                       \
    }                                           \
    v.bRnd = bRnd;                              \
    return v;                                   \
}

IMPLEMENT_FUNCTION(cos)
IMPLEMENT_FUNCTION(sin)
IMPLEMENT_FUNCTION(tan)
IMPLEMENT_FUNCTION(log)
IMPLEMENT_FUNCTION(sqrt)
IMPLEMENT_FUNCTION(exp)
IMPLEMENT_FUNCTION(floor)
IMPLEMENT_FUNCTION(ceil)

ostream& operator<<(ostream& out, const Data& d)
{
    if(d.isRnd()) out << "~";
    if(!d.bDef) out << "undefined";
    else
        switch(d.t) {
            case Data::generic_t: out << "g:" << d.data.ptr; break;
            case Data::double_t: out << d.data.d; break;
            case Data::int_t: out << d.data.n; break;
            case Data::list_t:
            {
                const vector<Data> *pl = d.getList();
                cout << "[";
                for(int i = 0; i < (int)pl->size(); i++)
                    cout << (*pl)[i] << " ";
                cout << "]";
                break;
            }
            case Data::expr_t: out << "e:" << d.data.pe; break;
#ifdef SUPPORT_OPENCV
            case Data::cvmat_t:
                cv::namedWindow("cout");
                cv::imshow("cout", *d.data.pmat);
                cv::waitKey(1);
                out << "cvmat" /* *d.data.pmat */; break;
#endif
        }
    return out;
}

string Data::to_s() const
{
    // TODO
    if(!bDef) return "?";
    else
        switch(t) {
            case Data::generic_t: return "ptr";
            case Data::double_t: return to_string((long double)data.d);
#ifdef WIN32
			case Data::int_t: return to_string((_Longlong)data.n);
#else
			case Data::int_t: return to_string(data.n);
#endif
            case Data::expr_t: return "E"; /*data.pe->to_s();*/
            case Data::list_t: return "List";
#ifdef SUPPORT_OPENCV
            case Data::cvmat_t: return "cv::Mat";
#endif
        }
	return "_ERROR_";
}


