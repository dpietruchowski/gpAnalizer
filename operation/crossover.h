#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "geneticoperation.h"

class Crossover : public GeneticOperation
{
public:
    Crossover();
    TreePtr reproduce(const std::vector<Tree*> parents);

private:
    virtual TreePtr crossover(Tree* parent1,
                              Tree* parent2) const = 0;
};

#endif // CROSSOVER_H
