#ifndef TERMINALSET_H
#define TERMINALSET_H

#include <cstdlib>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "typeFunc.h"

using namespace cv;

class terminalSet
{
public:
    static Mat_ptr nullImage;
private:
    static Mat_ptr inputImage;
public:
    terminalSet();
    Mat_ptr getRandomImage() const;
    void setInputImage(Mat_ptr inputImage);
};

#endif // TERMINALSET_H
