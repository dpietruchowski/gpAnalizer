#include "crossover.h"

Crossover::Crossover():
    GeneticOperation(2)
{

}

TreePtr Crossover::reproduce(const std::vector<Tree *> parents)
{
    if(parents.size() != 2)
        throw std::string("Mutation::reproduce: Too many parents");

    return crossover(parents[0], parents[1]);
}

