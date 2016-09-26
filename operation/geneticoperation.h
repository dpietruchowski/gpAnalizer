#ifndef GENETICOPERATION_H
#define GENETICOPERATION_H

#include "application/tree.h"

class GeneticOperation
{
public:
    GeneticOperation(int size);
    virtual ~GeneticOperation() {}
    int getSize() const;
    virtual TreePtr reproduce(const std::vector<Tree*> parents,
                              NodeGenerator* generator);

protected:
    int getRandomMutationPoint(Tree *parent) const;
    NodeGenerator* generator_;
private:
    virtual TreePtr reproduce(const std::vector<Tree*> parents) = 0;
private:
    int size_;
};

#endif // GENETICOPERATION_H
