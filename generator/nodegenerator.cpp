#include "nodegenerator.h"

NodeGenerator::NodeGenerator():
    objectCounter_(0)
{
}

NodePtr NodeGenerator::createRandomPtr()
{
    CreationCounter<CreateObjectFunction> counter = generator_.createRandom();
    CreateObjectFunction function = counter.callbackFunction;

    NodePtr object = function(counter.nCreation);
    objectCounter_++;
    return object;
}

void NodeGenerator::registerObject(double probability,
                                   NodeGenerator::CreateObjectFunction function)
{
    generator_.registerCallback(probability, function);
}

