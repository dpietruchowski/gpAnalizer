#ifndef MUTATION_H
#define MUTATION_H

#include "geneticoperation.h"

class Mutation : public GeneticOperation
{
public:
    Mutation();
    TreePtr reproduce(const std::vector<Tree*> parents);

private:
    virtual TreePtr mutate(Tree* parent) = 0;
};

#endif // MUTATION_H
