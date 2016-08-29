#include "subtreemutation.h"

GeneticOperation *SubtreeMutation::create()
{
    return new SubtreeMutation();
}

SubtreeMutation::SubtreeMutation()
{

}

TreePtr SubtreeMutation::mutate(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
    int subtreeDepth = parent->getSubtreeDepth(mutationPoint);
    Tree newSubtree(subtreeDepth, 0);
    newSubtree.initialize(FULL_INIT, *generator_);
    TreePtr offspring = move( parent->clone(0) );
    offspring->setSubtree(mutationPoint, *newSubtree.getRoot());

    return move(offspring);
}

