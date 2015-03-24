#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>

// #include <dirent.h>

#include "rand_expr.h"
#include "eiterator.h"

using namespace std;
using namespace cv;

#define HIST_SIZE (18 + 26)
void calcHSHist(Mat im, Mat &hist)
{
    double *dhist = new double[HIST_SIZE];
    Mat hsv;
    cvtColor(im, hsv, COLOR_BGR2HSV);
    for(int y = 0; y < im.rows; y++) {
        unsigned char *ptr = im.ptr<uchar>(y);
        for(int x = 0; x < im.cols; x++) {
            dhist[ptr[x * 3] / 10]++;
            dhist[18 + ptr[x * 3 + 1] / 10]++;
        }
    }
    for(int i = 0; i < HIST_SIZE; i++) {
        dhist[i] /= im.rows * im.cols;
    }
    hist = Mat(Size(HIST_SIZE, 1), CV_64FC1, dhist);
}

int main(int argc, const char * argv[])
{
    // string s = typeid(YourClass).name()
    srand((unsigned int)time(NULL) /*1232453*/), rand();
    Environment env;

    //Mat img = imread("fruits.jpg"/*, IMREAD_GRAYSCALE*/);
    //resize(img, img, Size(80, 80));

    /*List *imgs = new List();

    DIR *dir = opendir("pics");
    struct dirent *result;
    while((result = readdir(dir))) {
        if(!strstr(result->d_name, ".jpg") && !strstr(result->d_name, ".JPEG") ) continue;
        cout << result->d_name << endl;
        Mat im = imread(string("pics/") + string(result->d_name));
        Mat hist;
        calcHSHist(im, hist);
        *imgs << V(hist);
    }*/


    Expression *p = new Block();
    //Symbol f("f"), x("x"), y("y"), n("n"), im("im"), imx("imx"), imy("imy");
    //Symbol nClust("nClust"), centers("centers"), center("center"), im("im");
    //cv::Mat img = Mat::zeros(127, 128, CV_8UC1);
    //circle(img, Point(30, 30), 20, Scalar(255, 255, 255));

    cv::Mat img = imread("pic31.png", IMREAD_GRAYSCALE);
    cv::resize(img, img, Size(img.cols / 5, img.rows / 5));
    img = 255 - img;
    /*cv::Mat imgx, imgy;
    cv::GaussianBlur(img, img, Size(17, 17), 0);
    cv::Sobel(img, imgx, CV_32FC1, 1, 0);
    cv::Sobel(img, imgy, CV_32FC1, 0, 1);
    imgx = abs(imgx) + abs(imgy);
    imgx.convertTo(img, CV_8UC1);*/

    namedWindow("original");
    imshow("original", img);


#if 1
    Symbol im, imc, im0, n, circs, x, y, circ, r0;
    p = GPQuery(List() << include_libdefs()
                       //<< Define(n, V(27))
                       << Define(n, V(6))
                       << Define(circs,
                                 ::repeat(n, Lambda0(List() << V(Drawer::shape_circle)
                                                          << RndInt(img.cols + 10)-5
                                                          << RndInt(img.rows + 10)-5
                                // doesn't work correctly: r0 is substituted many times
                                // and then is changed independently
                                                     << /*RndInt(15) + 5*/ V(6)+RndInt(12)
                                                          << V(168) << V(-1))))
                       /*<< Define(im0, foldr(Lambda(circ, im,
                                       DrawCircle(im, circ[0], circ[1], circ[2], V(255), V(-1))),
                                circs, V(Mat::zeros(img.rows, img.cols, CV_8UC1))))*/
                       << Define(im0, Drawer(V(Mat::zeros(img.rows, img.cols, CV_8UC1)), circs))
                       //<< Define(imc, ::GaussianBlur(im0, V(13), V(0)) * 5)
                       << im0 //List(im0, circs)
                       << Log(MatDiff2(im0/*c*/, V(img))) * V(img.cols * img.rows)
                       ).reeval(&env, NULL, 0.005);
    cout << p->getValue() << endl;
#endif


    /*cout <<
    (::GaussianBlur(DrawCircle(V(Mat::zeros(img.rows, img.cols, CV_8UC1)),
                              V(50), V(50), V(10), V(255)),
                   V(13), V(0)) * 5).eval()->getValue();*/
    cv::waitKey();

    //p = p->reeval(&env);
    //cout << p->getChild(0)->getValue() << endl;


    
    
    return 0;
}


