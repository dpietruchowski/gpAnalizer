#ifndef COPY_H
#define COPY_H

#include "geneticoperation.h"

class Copy : public GeneticOperation
{
public:
    static GeneticOperation* create();
    Copy();
    TreePtr reproduce(const std::vector<Tree*> parents);
};

#endif // COPY_H
