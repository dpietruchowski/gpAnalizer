#include "subtreemutation.h"
#include <iostream>

using namespace std;

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
    // initialize has infinite loop if result == black or white
    if(subtreeDepth > 8) subtreeDepth = 8;
    Tree newSubtree(subtreeDepth, 0, parent->getImage());
    newSubtree.initialize(FULL_INIT, *generator_);
    TreePtr offspring = move( parent->clone(0) );
    offspring->setSubtree(mutationPoint, *newSubtree.getRoot());

    return move(offspring);
}

