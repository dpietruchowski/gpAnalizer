#ifndef HAMMING_H
#define HAMMING_H

#include "fitness.h"

class Hamming: public Fitness
{
public:
    static Fitness* create(const cv::Mat &referenceImage);
    Hamming(std::string referenceImageName);
    Hamming(const cv::Mat& referenceImage);

private:
    virtual int fitness(cv::Mat &A, cv::Mat &B) const;
};

#endif // HAMMING_H
