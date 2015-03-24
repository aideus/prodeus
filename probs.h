//
//  probs.h
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#ifndef __rand_expr__probs__
#define __rand_expr__probs__

#define PI 3.14159265359
double uniform0_samp();
int randint_samp(int n);
void randint_resamp(int &vi, int n, double tp);
double gaussian0_samp();
double gaussian_samp(double x0, double sigma);
void gaussian_resamp(double &vd, double x0, double sigma, double tp);
double gaussian_logp(double x, double x0, double sigma);


#endif /* defined(__rand_expr__probs__) */
