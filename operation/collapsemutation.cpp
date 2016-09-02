#include "collapsemutation.h"
#include <iostream>

using namespace std;

GeneticOperation *CollapseMutation::create()
{
    return new CollapseMutation();
}

CollapseMutation::CollapseMutation()
{

}

TreePtr CollapseMutation::mutate(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);

    TreePtr offspring = move( parent->clone(0) );
    int parentDepth = parent->getDepth();
    if(parentDepth > 3)
    {
        int subtreeDepth = parent->getSubtreeDepth(mutationPoint);
        while(((parentDepth - subtreeDepth) < 2) || (parentDepth == subtreeDepth))
        {
            mutationPoint = getRandomMutationPoint(parent);
            subtreeDepth = parent->getSubtreeDepth(mutationPoint);
        }
        //Changing radomly picked node with new term node.
        NodePtr newNode( TerminalNode::create(0) );
        offspring->setNode(mutationPoint, *newNode);
    }

    return move(offspring);
}

