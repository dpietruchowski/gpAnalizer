#ifndef NODEGENERATOR_H
#define NODEGENERATOR_H

#include "node/nodes.h"
#include "generator.h"

class NodeGenerator
{
public:
    typedef NodePtr (*CreateObjectFunction)(unsigned int);

public:
    NodeGenerator();
public:
    NodePtr createRandomPtr();
    void registerObject(double probability, CreateObjectFunction function);

private:
    Generator<CreateObjectFunction> generator_;
    int objectCounter_;
};

#endif // NODEGENERATOR_H
