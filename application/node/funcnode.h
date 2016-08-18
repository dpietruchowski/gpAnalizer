#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "node.h"
#include "application/functionset.h"

class funcNode: public node
{
private:
    func function;
    int argumentsNumber;
    std::string functionName;
public:
    funcNode(std::pair<funcId,func> p, std::string id);
    funcNode(std::pair<funcId,func> p);

    std::string getInfo();
    node_ptr copyAttribute();
    void showNode();
    void execute(const std::vector<cv::Mat> &src, cv::Mat &dst);
    int getArgumentsNumber();
};

#endif // FUNCNODE_H
