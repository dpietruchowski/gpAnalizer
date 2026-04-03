#include "mutation.h"
#include "../exceptions.h"

Mutation::Mutation():
    GeneticOperation(1)
{

}

TreePtr Mutation::reproduce(const std::vector<Tree *> parents)
{
    if(parents.size() != 1)
        throw InvalidArgumentException("Mutation::reproduce: Too many parents");

    return mutate(parents[0]);
}

