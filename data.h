//
//  data.h
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#ifndef __rand_expr__data__
#define __rand_expr__data__

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

#include "config.h"
#include "pointers.h"

#ifdef SUPPORT_OPENCV
#include "opencv2/opencv.hpp"
#endif


class Data {
    friend class RndInt_; // for efficient access
    friend class Gaussian;
public:
    enum DataType {
         undef_t,
	 double_t,
	 int_t,
	 list_t,
	 expr_t,
#ifdef SUPPORT_OPENCV
	 // we shoudn't have cvmat_t without opencv in order to not have 
	 //"cvmat_t not handled in switch" warrning
	 cvmat_t  
#endif
    };
   
    Data() { bRnd = false; t = undef_t; }
    
    //coping constructor and assigment operator
    Data(const Data&d); 
    Data& operator=(const Data&);
   
    bool isDef() const { return t != undef_t; }
    bool isRnd() const { return bRnd; }
    void forceRnd(const bool bR) { bRnd = bR; }
    ~Data();
    // todo: check if already defined
    Data& operator=(double d);
    Data& operator=(int n);
    Data& operator=(ExpressionP pExpr);
    
    void set_list_assume_ownership(std::vector<Data> *pL);
    
   
#ifdef SUPPORT_OPENCV
    //we don't have version with pointer to make sure that we copy it
    Data& operator=(const cv::Mat &mat);
#endif
    Data operator<(const Data &r) const;
    Data operator<=(const Data &r) const;
    Data operator>(const Data &r) const;
    Data operator>=(const Data &r) const;
    Data operator+(const Data &r) const;
    Data operator-(const Data &r) const;
    Data operator*(const Data &r) const;
    Data operator/(const Data &r) const;
    Data operator%(const Data &r) const;
    Data cos() const;
    Data sin() const;
    Data tan() const;
    Data log() const;
    Data sqrt() const;
    Data exp() const;
    Data floor() const;
    Data ceil() const;
    inline const DataType getType() const { return t; }
    inline const double getDouble() const {
        // if(!bDef) ...
        switch(t) {
            case double_t: return data.d;
            case int_t: return (double)data.n;
            default:
                return 0.;
        }
        /*TODO: if type if define*/
        /*TODO: conversion*/
    }
    inline const int getInt() const {
        // if(!bDef) ...
        switch(t) {
            case int_t: return data.n;
            case double_t: return (int)data.d; // todo: warning?
            default:
                return 0;
        }
        /*TODO: if type if define*/
        /*TODO: conversion*/
        return data.n;
    }
#ifdef SUPPORT_OPENCV
    inline const cv::Mat *getMat() const {
        if(t != cvmat_t) {
            return NULL;
            // throw "Error in accessing data as expression";
        }
        return data.pmat->get();
    }
#endif
   ExpressionP getExpression() const; 
    inline const std::vector<Data> *getList() const {
        if(t != list_t) {
            return NULL;
            // throw "Error in accessing data as expression";
        }
        /*TODO? conversion via Value?*/
        return data.l->get();
    }
    friend std::ostream& operator<<(std::ostream& out, const Data& d);
    virtual std::string to_s() const;
 private:
   //delete data if necessary end set t = t_undef
   void clear();
protected:
    union U {
        double d;
        int n;
        shared_ptr< vector<Data> > *l;
        ExpressionP *pe;
#ifdef SUPPORT_OPENCV
        shared_ptr<cv::Mat> *pmat;
#endif
    } data;
    bool bRnd;
    DataType t;
};


#endif /* defined(__rand_expr__data__) */
