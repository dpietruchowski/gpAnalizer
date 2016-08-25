#include "fitnessgenerator.h"

FitnessGenerator::FitnessGenerator()
{

}

void FitnessGenerator::setReferenceImage(const cv::Mat &img)
{
    referenceImage_ = img;
}

Fitness *FitnessGenerator::createRandomPtr()
{
    CreationCounter<CreateObjectFunction> counter = generator_.createRandom();
    CreateObjectFunction function = counter.callbackFunction;

    return function(referenceImage_);
}

void FitnessGenerator::registerObject(double probability, FitnessGenerator::CreateObjectFunction function)
{
    generator_.registerCallback(probability, function);
}

