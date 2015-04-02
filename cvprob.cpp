//
//  cvprob.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 01/03/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include "cvprob.h"
#include <cstdlib>

#ifdef SUPPORT_OPENCV

void GaussianBlur::calcValue(double tp, ExpressionP pPartner)
{
    const cv::Mat *pSrc = children[0]->getValue().getMat();
    int ksize = children[1]->getValue().getInt();
    double sigma = children[2]->getValue().getDouble();
    cv::Mat dst;
    cv::GaussianBlur(*pSrc, dst, cv::Size(ksize, ksize), sigma);
    v = dst;
    v.forceRnd(isChildrenRnd());
}

void DrawCircle::calcValue(double tp, ExpressionP pPartner)
{
    const cv::Mat *pSrc = children[0]->getValue().getMat();
    int x = children[1]->getValue().getInt();
    int y = children[2]->getValue().getInt();
    int rad = children[3]->getValue().getInt();
    int gray = children[4]->getValue().getInt(); // todo: RGB
	int thickness = children.size() > 5 ? children[5]->getValue().getInt() : 1;

    cv::Mat dst = pSrc->clone();
    cv::circle(dst, cv::Point(x, y), rad, cv::Scalar(gray, gray, gray), thickness);
    v = dst;
    v.forceRnd(isChildrenRnd());
}

void Drawer::calcValue(double tp, ExpressionP pPartner)
{
    const cv::Mat *pSrc = children[0]->getValue().getMat();
    cv::Mat dst = pSrc->clone();
    const std::vector<Data> *pObjects = children[1]->getValue().getList();
   if (pObjects == NULL)
     {
	cerr<<"Error | Drawer::calcValue | logical error 1"<<endl;
	exit(EXIT_FAILURE);
     }
    for(size_t i = 0; i < pObjects->size(); i++) {
        const std::vector<Data> *pShape = (*pObjects)[i].getList();
       if (pShape == NULL)
	 {
	    cerr<<"Error | Drawer::calcValue | logical error 2"<<endl;
	    cerr<<i<<" "<<(*pObjects)[i]<<endl;
	    exit(EXIT_FAILURE);
	 }
        int shapeType = (*pShape)[0].getInt();
        switch(shapeType) {
            case Shape::shape_circle:
                int x = (*pShape)[1].getInt();
                int y = (*pShape)[2].getInt();
                int rad = (*pShape)[3].getInt();
                int gray = (*pShape)[4].getInt(); // todo: RGB
                int thickness = pShape->size() > 5 ? (*pShape)[5].getInt() : 1;
                cv::circle(dst, cv::Point(x, y), rad, cv::Scalar(gray, gray, gray), thickness);
                break;
        }
    }
    v = dst;
    v.forceRnd(isChildrenRnd());
}

void MatDiff2::calcValue(double tp, ExpressionP pPartner)
{
    const cv::Mat *pMat1 = children[0]->getValue().getMat(),
                  *pMat2 = children[1]->getValue().getMat();
    // could be more efficient
    cv::Mat fl1, fl2;
    pMat1->convertTo(fl1, CV_64F);
    pMat2->convertTo(fl2, CV_64F);
    fl1 -= fl2;
    v = sqrt(fl1.dot(fl1) / (fl1.cols * fl1.rows * fl1.channels()));
    v.forceRnd(isChildrenRnd());
    // incorrect for unsigned?
    //cv::Mat mat = *pMat1 - *pMat2;
    //mat = mat.dot(mat);
}

#endif
