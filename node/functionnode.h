#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "node.h"
#include "functionset.h"

typedef int(* FunctionPtr) (const std::vector<cv::Mat>&, cv::Mat&);
struct FunctionId;

class FunctionNode : public Node
{
public:
    static FunctionSet& getFunctionSet();
    static NodePtr create(unsigned int geneNumber);

public:
    void execute(const std::vector<cv::Mat>&, cv::Mat& dst) const;
    void mutate();

private:
    FunctionNode(const NodeId& id, std::pair<FunctionId, FunctionPtr> p);
    FunctionNode(const FunctionNode& rhs);

private:
    virtual void writeNode(std::string& nodeString) const;
    virtual NodePtr cloneNode() const;

private:
    FunctionPtr function_;
    FunctionId functionId_;

    static FunctionSet functions_;
};

#endif // FUNCTIONNODE_H
