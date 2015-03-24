//
//  probs.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 25/01/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include "probs.h"

double uniform0_samp()
{
    return (double)rand()/RAND_MAX; // todo: good random
}

int randint_samp(int n)
{
    return rand() % n; // todo: good random
}

void randint_resamp(int &vi, int n, double tp)
{
    // todo? better distribution? (changes are rather small)
    int max_v = n - 1;
     int prev_v = vi;
     bool dirc = uniform0_samp() < 0.5; // direction of change (#f - left, #t - right)
     double sigma = tp * (dirc ? max_v - prev_v : prev_v);
     double delta = fabs(gaussian_samp(0, sigma));
     int new_v = (int)(prev_v + 0.5 + delta * (dirc ? 1 : -1));
     if(new_v < 0) new_v = 0;
     if(new_v > max_v) new_v = max_v;
     vi = new_v;
    // TODO: for images
    /*if(uniform0_samp() < tp) {
        vi = vi + (rand()%2*2 - 1)*10; if(vi >= n) vi = n-1; if(vi < 0) vi = 0;
        //vi = randint_samp(n);
    }*/
}



double gaussian0_samp()
{
    return cos(2 * PI * uniform0_samp()) * sqrt(-2 * log(1e-10 + uniform0_samp())); //??
}

double gaussian_samp(double x0, double sigma)
{
    return x0 + sigma * gaussian0_samp();
}

void gaussian_resamp(double &vd, double x0, double sigma, double tp)
{
    vd = gaussian_samp(vd, sigma * tp); // todo? biased towards x0?
}

// -log(p)
double gaussian_logp(double x, double x0, double sigma)
{
    double delta = (x - x0) / sigma;
    return log(sigma * sqrt(2. * PI)) + 0.5 * delta * delta;
}
