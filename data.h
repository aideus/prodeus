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
#include <memory.h>
#include <vector>

#include "config.h"

#ifdef SUPPORT_OPENCV
#include "opencv2/opencv.hpp"
#endif


class Data {
    friend class RndInt_; // for efficient access
    friend class Gaussian;
public:
    enum DataType {
        generic_t = 0,
        double_t  = 1,
        int_t     = 2,
        list_t    = 3,
        expr_t    = 4,
        cvmat_t   = 5};
    Data() { bDef = false; bRnd = false; t = generic_t; }
    const bool isDef() const { return bDef; }
    const bool isRnd() const { return bRnd; }
    void forceRnd(const bool bR) { bRnd = bR; }
    ~Data();
    // todo: check if already defined
    Data& operator=(double d) { data.d = d; bDef = true; bRnd = false; t = double_t; return *this; }
    Data& operator=(int n) { data.n = n; bDef = true; bRnd = false; t = int_t; return *this; }
    Data& operator=(class Expression *pExpr);
    Data& operator=(std::vector<Data> *pL);
#ifdef SUPPORT_OPENCV
    Data& operator=(cv::Mat *pMat);
    Data& operator=(const cv::Mat &mat);
#endif
    /*Data& operator=(const Data &from) {
        bDef = from.bDef;
        t = from.t;
        switch(t) {
            case list_t: data.l = from.data.l; break;
            default: memcpy(&data, &from.data, sizeof(data));
        }
        return *this;
    }*/
    //Data& operator=(std::vector<Data> l) { data.l = l; bDef = true; t = list_t; return *this; }
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
        if(!bDef || t != cvmat_t) {
            return NULL;
            // throw "Error in accessing data as expression";
        }
        return data.pmat;
    }
#endif
    inline const class Expression *getExpression() const {
        if(!bDef || t != expr_t) {
            return NULL;
            // throw "Error in accessing data as expression";
        }
        /*TODO? conversion via Value?*/
        return data.pe;
    }
    inline const std::vector<Data> *getList() const {
        if(!bDef || t != list_t) {
            return NULL;
            // throw "Error in accessing data as expression";
        }
        /*TODO? conversion via Value?*/
        return data.l;
    }
    inline void clear_if_list() { if(t == list_t) data.l->clear(); }
    friend std::ostream& operator<<(std::ostream& out, const Data& d);
    virtual std::string to_s() const;

protected:
    union U {
        U() { ptr = NULL; }
        ~U() { /*todo*/ }
        void *ptr;
        class Expression *pe;
        std::vector<Data> *l;
        double d;
        int n;
#ifdef SUPPORT_OPENCV
        cv::Mat *pmat;
#endif
    } data;
    bool bDef, bRnd;
    DataType t;
};


#endif /* defined(__rand_expr__data__) */
