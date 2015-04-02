//
//  basic_rnd.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 26/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include <iostream>
#include "rand_expr.h"
#include "probs.h"
#include "cvprob.h"

using namespace std;



void Flip_::calcValue(double tp, ExpressionP pPartner)
{
    if(!v.isDef() || uniform0_samp() < tp) {
        if(!children.empty()) {
            p = children[0]->getValue().getDouble();
        }
        v = (uniform0_samp() < p);
        v.forceRnd(true);
        logps = (v.getInt() ? -log(p) : -log(1. - p));
    }
}

void Gaussian::calcValue(double tp, ExpressionP pPartner)
{
    double x0 = children[0]->getValue().getDouble();
    double sigma = children[1]->getValue().getDouble();
    if(!v.isDef()) {
        v = gaussian_samp(x0, sigma);
    } else {
        gaussian_resamp(v.data.d, x0, sigma, tp);
    }
    logps = gaussian_logp(v.getDouble(), x0, sigma);
    v.forceRnd(true);
}


RndInt_ RndInt(int n) { return RndInt(V(n)); }
RndInt_ RndInt(const Expression &e) { return RndInt_(e, true); }

void RndInt_::calcValue(double tp, ExpressionP pPartner)
{
    int n = children[0]->getValue().getInt();
    if(pPartner && pPartner->name() == name() && pPartner->getValue().isDef()) {
        int perc = rand() % 100;
        //v.data.n = (v.data.n * perc + pPartner->getValue().data.n * (100 - perc)) / 100;
        if(perc < 50) v.data.n = pPartner->getValue().data.n;
    }
    if(!v.isDef()) {
        v = randint_samp(n);
    } else {
        randint_resamp(v.data.n, n, tp);
    }
    logps = log((double)n);
    v.forceRnd(true);
}



#ifdef SUPPORT_OPENCV
void MatRndInt::calcValue(double tp, ExpressionP pPartner)
{
    int rows = children[0]->getValue().getInt();
    int cols = children[1]->getValue().getInt();
    int type = children[2]->getValue().getInt();
    int n = children[3]->getValue().getInt();
    const cv::Mat *pIm0 = children.size() > 4 ? children[4]->getValue().getMat() : NULL;
    if(!v.isDef()) {
        cv::Mat *pMat = new cv::Mat(rows, cols, type);
        if(pIm0 == NULL) {
            EACH_MAT(pMat, (NULL), randint_samp(n));
        } else {
            // TODO: <unsigned char> can be incorrect... EACH_MAT2 ?
            EACH_MAT(pMat, (NULL), pIm0->at<unsigned char>(y, x));
        }
        v = *pMat;
        delete pMat;
    } else {
        cv::Mat *pMat = const_cast<cv::Mat *>(v.getMat());
        int t; // MatRndInt creates integer matrix
        if(rows == pMat->rows && cols == pMat->cols && type == pMat->type()) {
            if(pIm0 == NULL) {
                EACH_MAT(pMat, (t = (int)p[x], randint_resamp(t, n, tp)), t);
            } else {
                // use pIm0->at<unsigned char>(y, x) ??
                EACH_MAT(pMat, (t = (int)p[x], randint_resamp(t, n, tp)), t);
            }
        } else {
            // TODO
        }
    }
	// TODO
    logps = 0.;
    v.forceRnd(true);
}

void MatGaussian::calcValue(double tp, ExpressionP pPartner)
{
    int rows = children[0]->getValue().getInt();
    int cols = children[1]->getValue().getInt();
    int type = children[2]->getValue().getInt();
    double x0d = 0.;
    const cv::Mat *x0m = NULL;
    bool bDoubleX0 = children[3]->getValue().getType() != Data::cvmat_t;
    if(bDoubleX0) {
      x0d = children[3]->getValue().getDouble();
    } else {
      x0m = children[3]->getValue().getMat();
    }
    double sigma = children[4]->getValue().getDouble();
    if(!v.isDef()) {
        cv::Mat *pMat = new cv::Mat(rows, cols, type);
        if(bDoubleX0) {
            EACH_MAT(pMat, NULL, gaussian_samp(x0d, sigma));
        } else {
            // TODO: <unsigned char> can be incorrect... EACH_MAT2 ?
            EACH_MAT(pMat, NULL, gaussian_samp(x0m->at<unsigned char>(y, x), sigma));
        }
        v = *pMat;
       delete pMat;
    } else {
        cv::Mat *pMat = const_cast<cv::Mat *>(v.getMat());
        // // we should not destroy previous value during re-evaluation!
        double t;
        if(rows == pMat->rows && cols == pMat->cols && type == pMat->type()) {
            EACH_MAT(pMat, (t = p[x], gaussian_resamp(t, x0d, sigma, tp)), t);
        } else {
            // TODO: <unsigned char> can be incorrect... EACH_MAT2 ?
            EACH_MAT(pMat, (t = p[x], gaussian_resamp(t, x0m->at<unsigned char>(y, x), sigma, tp)), t);
        }
    }
    // TODO
	logps = 0.;
    v.forceRnd(true);
}
#endif



