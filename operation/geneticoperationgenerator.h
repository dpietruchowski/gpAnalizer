#ifndef GENETICOPERATIONGENERATOR_H
#define GENETICOPERATIONGENERATOR_H

#include "geneticoperation.h"

class GeneticOperationGenerator
{
public:
    typedef GeneticOperation* (*CreateObjectFunction)();
    GeneticOperationGenerator();
public:
    GeneticOperation* createRandomPtr();
    void registerObject(double probability, CreateObjectFunction function);

private:
    Generator<CreateObjectFunction> generator_;
};

#endif // GENETICOPERATIONGENERATOR_H
