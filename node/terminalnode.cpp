#include "terminalnode.h"

#include <opencv2/highgui/highgui.hpp>

using namespace std;

NodePtr TerminalNode::create(unsigned int geneNumber)
{
    NodeId id = {TERMINAL_NODE, geneNumber, 0};
    return NodePtr(new TerminalNode(id));
}

NodePtr TerminalNode::createFromXml(const tinyxml2::XMLElement *node)
{
    return NodePtr(new TerminalNode(node));
}

TerminalNode::TerminalNode(const NodeId &id):
    Node(id, 0)
{

}

TerminalNode::TerminalNode(const tinyxml2::XMLElement *node):
    Node(node)
{
}

TerminalNode::TerminalNode(const TerminalNode &rhs):
    Node(rhs)
{
}

void TerminalNode::writeNode(string &nodeString) const
{
    nodeString += "input";
}

NodePtr TerminalNode::cloneNode() const
{
    NodePtr cloned( new TerminalNode(*this) );
    return cloned;
}

