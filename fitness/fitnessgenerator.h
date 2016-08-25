#ifndef FITNESSGENERATOR_H
#define FITNESSGENERATOR_H

#include "../generator/generator.h"
#include "fitness.h"

class FitnessGenerator
{
public:
    typedef Fitness* (*CreateObjectFunction)(const cv::Mat& image);
public:
    FitnessGenerator();
    void setReferenceImage(const cv::Mat& img);
public:
    Fitness* createRandomPtr();
    void registerObject(double probability, CreateObjectFunction function);

private:
    Generator<CreateObjectFunction> generator_;
    cv::Mat referenceImage_;
};

#endif // FitnessGENERATOR_H
