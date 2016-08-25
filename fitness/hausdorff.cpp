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
    vector<Point> a, b, c, d;

    this->transformImages(A, B);

    if(countNonZero(A) == 0)
        return 10000000;
    findNonZero(A, a);
    findNonZero(B, b);

    return this->maxDistance(a, b);
}

void Hausdorff::transformImages(Mat &, Mat &) const
{
    //do nothing;
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

int Hausdorff::maxDistance(const vector<Point> &a,
                           const vector<Point> &b) const
{
    int maxDistance = distance(a, b);
    int maxDistBA = distance(b, a);
    int maxDist = max(maxDistance, maxDistBA);

    return maxDist;
}
