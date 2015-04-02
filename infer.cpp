//
//  infer.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 04/02/15.
//  Copyright (c) 2015 Alexey Potapov, Vita Batishcheva. All rights reserved.
//

#include <string>
#include <math.h>
#include <algorithm>
#include "rand_expr.h"
#include "probs.h"

AnnealingQuery_ AnnealingQuery(const Expression &prog) { return AnnealingQuery_(prog, true); }
GPQuery_        GPQuery        (const Expression &prog) { return GPQuery_      (prog, true); }

AnnealingQuery_P sAnnealingQuery(const Expression &prog) { return make_shared<AnnealingQuery_>(prog, true); }
GPQuery_P        sGPQuery       (const Expression &prog) { return make_shared<GPQuery_>       (prog, true); }


ExpressionP AnnealingQuery_::reeval(Environment* pEnv, ExpressionP pPartner, double tp, bool bForceEval)
{
    double min_v = 1e+50;
    ExpressionP prog_best = ExpressionP(); //empty pointer
   
    AnnealingQuery_P pEvaluated = static_pointer_cast<AnnealingQuery_>(shared_from_this());
    if(bForceEval || !bEvaluated || v.isRnd()) {
        ExpressionP prog = children[0]->reeval(pEnv,
                                               ExpressionP(), // TODO?????
                                               tp, bForceEval);
        // double min_v = 1e+50;
#ifdef VERBOSE_EVAL
        cout << "Evaluating AnnealingQuery" << endl;
#endif
        for(int i = 0; i < 5000; i++) {
            double tt = tp / sqrt(i + 1.);
            for(int j = 0; j < 100; j++) {
                ExpressionP prog_ = prog->reeval(pEnv,
                                                 ExpressionP(), //TODO?????
                                                 tt, false /*it is re-evaluated internally*/);
                double dE =
                    /*log*/(prog_->getChild(prog_->getChildrenNum()-1)->getValue().getDouble()) -
                    /*log*/(prog->getChild(prog->getChildrenNum()-1)->getValue().getDouble());
                dE += prog_->getRecLogp();
				dE -= prog->getRecLogp();
                double p = 1 / (1 + exp(dE / tt));
                if(uniform0_samp() < p/* + tt*/) {
//double vvv = prog_->getChild(prog_->getChildrenNum()-1)->getValue().getDouble();
//cout << prog_->getChild(prog_->getChildrenNum()-1)->getValue() << endl;
cout
 << prog_->getChild(prog_->getChildrenNum()-2)->getValue() << " : "
 << prog_->getChild(prog_->getChildrenNum()-1)->getValue()
 << "(+" << prog_->getRecLogp() << ")" << endl;
//cout << "Try: " << prog_->getChild(prog_->getChildrenNum()-2)->getValue() << endl;
//cv::waitKey(1);
                    // delete prog;
                    prog = prog_;
                    // todo: delete
                    break;
                } // else delete prog_;
            }
            double dv = prog->getChild(prog->getChildrenNum()-1)->getValue().getDouble();
            if(dv < min_v || prog_best == NULL) { min_v = dv; prog_best = prog; }
        }
        // pEvaluated->children.push_back(prog_best); // todo
        pEvaluated->v = prog_best->getChild(prog_best->getChildrenNum()-2)->getValue();
    }
#ifdef VERBOSE_EVAL
    else { cout << "Skipping: "; }
    cout << "...Optimize = " << pEvaluated->getValue() << endl;
#endif
    return pEvaluated;
}

bool cmp(ExpressionP p1, ExpressionP p2) {
    // TODO: inefficient, since it is computed on each comparison while sorting
    return p1->getChild(p1->getChildrenNum()-1)->getValue().getDouble()/* + p1->getRecLogp()*/ <
           p2->getChild(p2->getChildrenNum()-1)->getValue().getDouble()/* + p2->getRecLogp()*/;
}

ExpressionP GPQuery_::reeval(Environment* pEnv, ExpressionP pPartner, double tp, bool bForceEval)
{
    GPQuery_P pEvaluated = dynamic_pointer_cast<GPQuery_>(shared_from_this());
    int nPop = 300;
    vector<ExpressionP> pop;
//    if(bForceEval || !bEvaluated || v.isRnd()) {
    for(int i = 0; i < nPop; i++) {
        pop.push_back(children[0]->reeval(pEnv, ExpressionP(), 1., bForceEval));
    }
    for(int j = 0; j < 100; j++) {
        for(int i = 0; i < nPop*3; i++) {
            pop.push_back(pop[rand()%nPop]->reeval(pEnv, pop[rand()%nPop], tp, false));
        }
        sort(pop.begin(), pop.end(), cmp);
        for(int i = 0; i < nPop*3; i++) {
            pop.pop_back();
        }
        cout
        << pop[0]->getChild(pop[0]->getChildrenNum()-2)->getValue() << " : "
        << pop[0]->getChild(pop[0]->getChildrenNum()-1)->getValue()
        << "(+" << pop[0]->getRecLogp() << ")" << endl;
        
        /*char s[512];
        sprintf(s, "sol_%d.png", j);
        cv::imwrite(s, *pop[0]->getChild(pop[0]->getChildrenNum()-2)->getValue().getMat());*/
    }
//    cv::imwrite("sol_last.png", *pop[0]->getChild(pop[0]->getChildrenNum()-2)->getValue().getMat());
    pEvaluated->v = pop[0]->getChild(pop[0]->getChildrenNum()-2)->getValue();
    return pEvaluated;
}

