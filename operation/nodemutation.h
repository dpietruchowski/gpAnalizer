#ifndef NODEMUTATION_H
#define NODEMUTATION_H

#include "mutation.h"

class NodeMutation : public Mutation
{
public:
    static GeneticOperation* create();
    NodeMutation();
private:
    TreePtr mutate(Tree* parent);
};

#endif // NODEMUTATION_H
