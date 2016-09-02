#include "geneticoperation.h"

GeneticOperation::GeneticOperation(int size):
    size_(size)
{

}

int GeneticOperation::getSize() const
{
    return size_;
}

TreePtr GeneticOperation::reproduce(const std::vector<Tree *> parents,
                                    NodeGenerator *generator)
{
    generator_ = generator;

    return reproduce(parents);
}

int GeneticOperation::getRandomMutationPoint(Tree *parent) const
{
    int mutationPoint = 1 + rand() % (parent->getSize() - 1);
        //Randomly picking node which isnt terminal node
    for (int i = 0; i < parent->getSize() * 2; i++)
    {
        if ( parent->getNode(mutationPoint)->getId().type == TERMINAL_NODE )
            mutationPoint = 1 + rand() % (parent->getSize() - 1);
        else
            break;
    }
    return mutationPoint;
}

