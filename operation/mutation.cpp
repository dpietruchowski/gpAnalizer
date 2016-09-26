#include "mutation.h"

Mutation::Mutation():
    GeneticOperation(1)
{

}

TreePtr Mutation::reproduce(const std::vector<Tree *> parents)
{
    if(parents.size() != 1)
        throw std::string("Mutation::reproduce: Too many parents");

    return mutate(parents[0]);
}

