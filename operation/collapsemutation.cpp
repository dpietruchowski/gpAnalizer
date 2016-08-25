#include "collapsemutation.h"

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
        //Changing radomly picked node with new term node.
    NodePtr newNode( TerminalNode::create(0) );
    TreePtr offspring = move( parent->clone(0) );
    offspring->setNode(mutationPoint, *newNode);

    return move(offspring);
}

