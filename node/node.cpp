#include "node.h"
#include "terminalnode.h"

#include <utility>
#include <limits>

using namespace std;
using namespace tinyxml2;

Node::Node(const NodeId& id, int size):
    id_(id), size_(size), nClones_(new int(0))
{
}

Node::Node(const XMLElement *node):
    nClones_(new int(0))
{
    load(node);
}

void Node::addChild(NodePtr child)
{
    if(isValid() == true)
        throw string("Node::addChild: Cannot add child");

    children_.push_back( move(child) );
}

void Node::setChild(int i, NodePtr child)
{
    if(isValid() == false)
        throw string("Node::setChild: Cannot set child");

    if(i > (size_ - 1) && i < 0)
        throw string("Node::setChild: Cannot set child");

    children_[i] = move(child);
}

Node *Node::getChild(int i) const
{
    if(isValid() == false)
        throw string("Node::getChild: Cannot get child");

    if( (i > (size_ - 1)) && (i < 0) )
        throw string("Node::getChild: Cannot get child");

    return children_[i].get();
}

NodePtr Node::cloneChild(int i)
{
    if(isValid() == false)
        throw string("Node::getChild: Cannot return child");

    if( (i > (size_ - 1)) && (i < 0) )
        throw string("Node::getChild: Cannot return child");

    return children_[i]->clone();
}

void Node::giveChildren(Node &rhs)
{
    int lowerSize = size_ < rhs.size_ ? size_ : rhs.size_;
    int i = 0;
    for(;i < lowerSize; ++i)
        children_.push_back(move(rhs.children_[i]));
    for(;i < size_; ++i)
        children_.push_back(TerminalNode::create(0));

    rhs.children_.clear();
}

int Node::getSize() const
{
    return size_;
}

int Node::getActualSize() const
{
    return static_cast<int>(children_.size());
}

bool Node::isValid() const
{
    return getSize() == getActualSize();
}

const NodeId &Node::getId() const
{
    return id_;
}

string Node::write() const
{
    string nodeString;
    nodeString += id_.toString() + " ";
    nodeString += "| ";
    nodeString += to_string(size_) + " ";

    writeNode(nodeString);

    return nodeString;
}

XMLElement *Node::save(XMLDocument &doc) const
{
    XMLElement* node = doc.NewElement("Node");
    id_.saveAttribute(node);
    node->SetAttribute("size", size_);

    save(node);
    return node;
}

NodePtr Node::clone() const
{
    return cloneNode();
}

Node::Node(const Node &rhs):
    id_(rhs.id_), size_(rhs.size_)
{
    nClones_ = rhs.nClones_;
    (*nClones_)++;

    id_ = rhs.id_;

    id_.cloneNumber = *nClones_;
}

void Node::load(const XMLElement *node)
{
    id_.loadAttribute(node);
    size_ = node->IntAttribute("size");
}
