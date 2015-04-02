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

// todo: check if it is needed to be copied in this way
Data::Data(const Data&other) 
{
   t = undef_t;
   operator=(other);
}
//                                                             
Data& Data::operator=(const Data& other)
{
   clear();
   switch(other.t)
     {
      case undef_t: 
      case double_t:
      case int_t:
	data = other.data;
	break;
      
      case list_t:
	//!!! ATTENTION !!!!
	//we simply copy smart pointer here!!!
	//we DO NOT copy data itself
	data.l = new shared_ptr< vector<Data> >(*(other.data.l));
	break;
      
      case expr_t:
	//!!! ATTENTIONON !!!
	//we simply copy smart pointer here!!!
	//we DO NOT copy data itself
	data.pe = new ExpressionP(*(other.data.pe)); 
	break;
	
#ifdef SUPPORT_OPENCV
      case cvmat_t:
	//!!! ATTENTIONON !!!
	//we simply copy smart pointer here!!!
	//we DO NOT copy data itself
	data.pmat = new shared_ptr<cv::Mat>(*(other.data.pmat)); 
	break;
#endif
      default:
	cout<<"Logic error | Data::operator= | unknown type"<<endl;
	exit(EXIT_FAILURE);
	break;
     }
   t = other.t;
   bRnd = other.bRnd;
   
   return *this;
}
//                                                                    
Data::~Data()
{
   clear();
}
//                                                                                 
Data& Data::operator=(double d) 
{ 
   clear();
   data.d = d; 
   bRnd = false; 
   t = double_t; 
   return *this; 
}
//                                                                                 
Data& Data::operator=(int n) 
{ 
   clear();
   data.n = n; 
   bRnd = false; 
   t = int_t; 
   return *this; 
}

Data& Data::operator=(ExpressionP pExpr)
{
   clear();
   data.pe = new ExpressionP(pExpr);
   t = expr_t;
   return *this;
}

void Data::set_list_assume_ownership(vector<Data> *pL)
{
   clear();
   data.l = new shared_ptr< vector<Data> >(pL);
   bRnd = false;
   for(vector<Data>::iterator it = pL->begin(); it != pL->end(); ++it)
     bRnd = bRnd || it->bRnd;
   t = list_t;
}

#ifdef SUPPORT_OPENCV
//Data& Data::operator=(cv::Mat *pMat)
//{
//clear();
//    data.pmat = new cv::Mat(pMat->clone()); // Data should be copied
//    t = cvmat_t;
//    bDef = true;
//    return *this;
//}

Data& Data::operator=(const cv::Mat &mat)
{
   clear();
   data.pmat = new shared_ptr<cv::Mat>(new cv::Mat(mat.clone())); // Data should be copied
   t = cvmat_t;
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
        if(r.t == cvmat_t) v = data.d Op **r.data.pmat;     \
    }                                               \
    if(t == int_t) {                                \
        if(r.t == double_t) v = data.n Op r.data.d; \
        if(r.t == int_t) v = data.n Op r.data.n;    \
        if(r.t == cvmat_t) v = data.n Op **r.data.pmat;     \
    }                                               \
    if(t == cvmat_t) {                              \
        if(r.t == double_t) v = **data.pmat Op r.data.d;    \
        if(r.t == int_t) v = **data.pmat Op r.data.n;       \
        if(r.t == cvmat_t) v = **data.pmat Op **r.data.pmat;  /*check sizes?*/ \
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
   switch(d.t) {
    case Data:: undef_t: 
      out << "undefined";
      break;
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
    case Data::expr_t: out << "e:" << *(d.data.pe); break;
#ifdef SUPPORT_OPENCV
    case Data::cvmat_t:
      cv::namedWindow("cout");
      cv::imshow("cout", **d.data.pmat);
      cv::waitKey(1);
      out << "cvmat" /* **d.data.pmat */; break;
#endif
   }
   return out;
}
//                                                                          
ExpressionP Data::getExpression() const 
{
   if(t != expr_t) 
     {
	return NULL;
	// throw "Error in accessing data as expression";
     }
   //         /*TODO? conversion via Value?*/
   return *(data.pe);
}

string Data::to_s() const
{
   switch(t) {
    case Data::undef_t: return "?";
      //            case Data::generic_t: return "ptr";
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
//                                                                           
void Data::clear()
{
   switch(t) 
     {
	  
      case list_t:  
	delete data.l;
	break;
      case expr_t: 
	delete data.pe;
	break;
#ifdef SUPPORT_OPENCV
      case cvmat_t:
	delete data.pmat;
	break;
#endif
      case undef_t: case double_t: case int_t: //do nothing
	break;
     }
   t = undef_t;
}
//                                                                           
