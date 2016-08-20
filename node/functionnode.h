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
    static NodePtr createFromXml(const tinyxml2::XMLElement *node);

public:
    void execute(const std::vector<cv::Mat>&, cv::Mat& dst) const;
    void mutate();

private:
    FunctionNode(const NodeId& id, std::pair<FunctionId, FunctionPtr> p);
    FunctionNode(const tinyxml2::XMLElement *node,
                 std::pair<FunctionId, FunctionPtr> p);
    FunctionNode(const FunctionNode& rhs);

private:
    virtual void writeNode(std::string& nodeString) const;
    virtual NodePtr cloneNode() const;
    virtual void save(tinyxml2::XMLDocument& doc,
                      tinyxml2::XMLElement *node) const;
    virtual void save(tinyxml2::XMLElement *node) const;

private:
    FunctionPtr function_;
    FunctionId functionId_;

    static FunctionSet functions_;
};

#endif // FUNCTIONNODE_H
