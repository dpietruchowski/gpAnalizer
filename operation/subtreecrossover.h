#ifndef SUBTREECROSSOVER_H
#define SUBTREECROSSOVER_H

#include "crossover.h"

class SubtreeCrossover : public Crossover
{
public:
    static GeneticOperation* create();
    SubtreeCrossover();

private:
    TreePtr crossover(Tree* parent1, Tree* parent2) const;
    TreePtr directCrossover(Tree* parent1, Tree* parent2) const;
};

#endif // SUBTREECROSSOVER_H
