#include "functionnode.h"

FunctionSet FunctionNode::functions_ = FunctionSet();

FunctionSet &FunctionNode::getFunctionSet()
{
    return functions_;
}

NodePtr FunctionNode::create(unsigned int geneNumber)
{
    NodeId id = {FUNCTION_NODE, geneNumber, 0};
    return NodePtr(new FunctionNode(id, functions_.getRandomFunction()));
}

NodePtr FunctionNode::createFromXml(const XMLElement *node)
{
    const char* name = node->Attribute("name");
    std::string functionName = std::string(name);
    return NodePtr(new FunctionNode(node, functions_.getFunction(functionName)));
}

void FunctionNode::execute(const std::vector<cv::Mat> &src, cv::Mat &dst) const
{
    function_(src, dst);
}

void FunctionNode::mutate()
{
    std::pair<FunctionId, FunctionPtr> p = FunctionNode::functions_.getRandomFunction(functionId_.nArguments);
    functionId_ = p.first;
    function_ = p.second;
}

FunctionNode::FunctionNode(const NodeId &id, std::pair<FunctionId, FunctionPtr> p):
    Node(id, p.first.nArguments), function_(p.second), functionId_(p.first)
{
}

FunctionNode::FunctionNode(const XMLElement *node,
                           std::pair<FunctionId, FunctionPtr> p):
    Node(node), function_(p.second), functionId_(p.first)
{
}

FunctionNode::FunctionNode(const FunctionNode &rhs):
    Node(rhs), function_(rhs.function_), functionId_(rhs.functionId_)
{
}

void FunctionNode::writeNode(std::string &nodeString) const
{
    nodeString += functionId_.name;
    nodeString += " ";
    nodeString += std::to_string(functionId_.nArguments);
}

NodePtr FunctionNode::cloneNode() const
{
    NodePtr cloned( new FunctionNode(*this) );
    return cloned;
}

void FunctionNode::save(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement *node) const
{
    tinyxml2::XMLElement *functionid = doc.NewElement("FunctionId");
    functionid->SetAttribute("name", functionId_.name.c_str());
    node->InsertEndChild(functionid);
}

void FunctionNode::save(tinyxml2::XMLElement *node) const
{
    node->SetAttribute("name", functionId_.name.c_str());
}
