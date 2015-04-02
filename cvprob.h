//
//  cvprob.h
//  rand_expr
//
//  Created by Alexey Potapov on 01/03/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__cvprob__
#define __rand_expr__cvprob__

#include "config.h"
#include "expression.h"
#include "value.h"

#ifdef SUPPORT_OPENCV
#define EACH_MAT(pMat, proc, assign) {                                         \
    int rows = (pMat)->rows, cols = (pMat)->cols;                              \
    int nch = (pMat)->channels(), szrow = nch * cols, depth = (pMat)->depth(); \
    for(int y = 0; y < rows; y++) {             \
        if(depth == CV_64F) {                   \
            double *p = (pMat)->ptr<double>(y); \
            for(int x = 0; x < szrow; x++) {    \
                proc;                           \
                p[x] = (assign);                \
            }                                   \
        } else if(depth == CV_32F) {            \
            float *p = (pMat)->ptr<float>(y);   \
            for(int x = 0; x < szrow; x++) {    \
                proc;                           \
                p[x] = (float)(assign);         \
            }                                   \
        } else if(depth == CV_32S) {            \
            int *p = (pMat)->ptr<int>(y);       \
            for(int x = 0; x < szrow; x++) {    \
                proc;                           \
                p[x] = (int)(assign);           /* no cast saturate? */ \
            }                                   \
        } else if(depth == CV_16S) {            \
            short *p = (pMat)->ptr<short>(y);   \
            for(int x = 0; x < szrow; x++) {    \
                proc;                           \
                p[x] = cv::saturate_cast<short>(assign);          \
            }                                   \
        } else if(depth == CV_16U) {            \
            unsigned short *p = (pMat)->ptr<unsigned short>(y);   \
            for(int x = 0; x < szrow; x++) {    \
                proc;                           \
                p[x] = cv::saturate_cast<unsigned short>(assign); \
            }                                   \
        } else if(depth == CV_8S) {             \
            char *p = (pMat)->ptr<char>(y);     \
            for(int x = 0; x < szrow; x++) {    \
                proc;                           \
                p[x] = cv::saturate_cast<char>(assign);          \
            }                                   \
        } else if(depth == CV_8U) {             \
            unsigned char *p = (pMat)->ptr<unsigned char>(y);    \
            for(int x = 0; x < szrow; x++) {    \
                proc;                           \
                p[x] = cv::saturate_cast<unsigned char>(assign); \
            }                                   \
        }                                       \
    }                                           \
}

class GaussianBlur: public Expression {
public:
    GaussianBlur(const cv::Mat &src, int ksize, double sigma): Expression() {
        children.push_back(make_shared<Value>(src));
        children.push_back(make_shared<Value>(ksize));
        children.push_back(make_shared<Value>(sigma));
    }
    GaussianBlur(const Expression &src, const Expression &ksize, const Expression &sigma):
        Expression(src, ksize, sigma) { }
    virtual ExpressionP clone() const { return make_shared<GaussianBlur>(*this); }
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "GaussianBlur"; }
    
};

class DrawCircle: public Expression {
public:
    DrawCircle(const cv::Mat &src, int x, int y, int rad, int gray): Expression() {
        children.push_back(make_shared<Value>(src));
        children.push_back(make_shared<Value>(x));
        children.push_back(make_shared<Value>(y));
        children.push_back(make_shared<Value>(rad));
        children.push_back(make_shared<Value>(gray));
    }
    DrawCircle(const cv::Mat &src, int x, int y, int rad, int gray, int thickness): Expression() {
        children.push_back(make_shared<Value>(src));
        children.push_back(make_shared<Value>(x));
        children.push_back(make_shared<Value>(y));
        children.push_back(make_shared<Value>(rad));
        children.push_back(make_shared<Value>(gray));
		children.push_back(make_shared<Value>(thickness));
    }
    DrawCircle(const Expression &src, const Expression &x, const Expression &y,
               const Expression &rad, const Expression &gray):
        Expression(src, x, y, rad, gray) { }
    DrawCircle(const Expression &src, const Expression &x, const Expression &y,
               const Expression &rad, const Expression &gray, const Expression &thickness):
        Expression(src, x, y, rad, gray, thickness) { }
    virtual ExpressionP clone() const { return make_shared<DrawCircle>(*this); }
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "DrawCircle"; }
    
};

class Drawer: public Expression {
public:
    enum Shape {
        shape_circle = 0
    };
    Drawer(const Expression &image, const Expression &objects): Expression(image, objects) {}
    virtual ExpressionP clone() const { return make_shared<Drawer>(*this); }
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "Drawer"; }
};

class MatDiff2: public Expression {
public:
    MatDiff2(const cv::Mat &mat1, const cv::Mat &mat2): Expression() {
        children.push_back(make_shared<Value>(mat1));
        children.push_back(make_shared<Value>(mat2));
    }
    MatDiff2(const Expression &mat1, const Expression &mat2): Expression(mat1, mat2) { }
    virtual ExpressionP clone() const { return make_shared<MatDiff2>(*this); }
    virtual void calcValue(double tp = 1.0, ExpressionP pPartner = ExpressionP());
    virtual string name() const { return "MatDiff2"; }
};

#endif /* defined(__rand_expr__cvprob__) */

#endif
