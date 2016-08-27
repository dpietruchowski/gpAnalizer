#include "hausdorff.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Fitness *Hausdorff::create(const cv::Mat& referenceImage)
{
    return new Hausdorff(referenceImage);
}

Hausdorff::Hausdorff(string referenceImageName):
    Fitness(referenceImageName)
{
}

Hausdorff::Hausdorff(const Mat &referenceImage):
    Fitness(referenceImage)
{

}

int Hausdorff::fitness(Mat &A, Mat &B) const
{
    int distanceAB = distance(A, B);
    int distanceBA = distance(B, A);

    return max(distanceAB, distanceBA);
}

void Hausdorff::transformImages(Mat &A, Mat &B) const
{
    Mat sumAB;

    bitwise_or(A, B, sumAB); // A sum B
    bitwise_not(sumAB, sumAB);

    bitwise_or(A, sumAB, A); // roznica A - A sum B

    bitwise_not(A, A);
    bitwise_not(B, B);
}

void Hausdorff::randomizePoints(std::vector<Point> &,
                                std::vector<Point> &) const
{
    // do nothing
}

int Hausdorff::distance(const vector<Point> &a,
                        const vector<Point> &b) const
{
    int maxDistance = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        int minAB = 10000000;
        for (size_t j = 0; j < b.size(); j++)
        {
           int dx = abs(a[i].x - b[j].x);
           int dy = abs(a[i].y - b[j].y);
           int tmpDst = dx*dx + dy*dy;

           if (tmpDst < minAB)
               minAB = tmpDst;

           if (tmpDst < maxDistance)
               break;

        }
        if(minAB > maxDistance)
            maxDistance = minAB;
    }

    return maxDistance;
}

int Hausdorff::distance(const Mat &A, const Mat &B) const
{
    Mat C = A.clone();
    Mat D = B.clone();

    transformImages(C, D);

    vector<Point> a, b;
    findNonZero(C, a);
    findNonZero(D, b);

    randomizePoints(a, b);

    return distance(a, b);
}
