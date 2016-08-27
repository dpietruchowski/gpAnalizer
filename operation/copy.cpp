#include "copy.h"

GeneticOperation *Copy::create()
{
    return new Copy();
}

Copy::Copy():
    GeneticOperation(1)
{

}

TreePtr Copy::reproduce(const std::vector<Tree *> parents)
{
    return parents[0]->clone(0);
}

