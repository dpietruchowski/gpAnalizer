#include "nodemutation.h"

GeneticOperation *NodeMutation::create()
{
    return new NodeMutation();
}

NodeMutation::NodeMutation()
{

}

TreePtr NodeMutation::mutate(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);

    TreePtr offspring = move( parent->clone(0) );
    offspring->getNode(mutationPoint)->mutate();

    return move(offspring);
}

