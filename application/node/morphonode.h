#ifndef MORPHONODE_H
#define MORPHONODE_H

#include "node.h"

class morphoNode: public node
{
private:
    morpho operation;
    int argumentsNumber;
    std::string operationName;
    morphologyParameters morph;
public:
    morphoNode(std::pair<funcId,morpho> p, std::string id, morphologyParameters morph);
    morphoNode(std::pair<funcId,morpho> p, morphologyParameters morph);

    std::string getInfo();
    node_ptr copyAttribute();
    void showNode();
    void execute(const std::vector<cv::Mat> &src, cv::Mat &dst);
    int getArgumentsNumber();
};

#endif // MORPHONODE_H
