#ifndef ARITY2CROSSOVER_H
#define ARITY2CROSSOVER_H

#include "crossover.h"

class Arity2Crossover : public Crossover
{
public:
    static GeneticOperation* create();
    Arity2Crossover();

private:
    TreePtr crossover(Tree* parent1, Tree* parent2) const;

};

#endif // ARITY2CROSSOVER_H
