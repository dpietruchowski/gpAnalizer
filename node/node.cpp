#include "node.h"
#include "terminalnode.h"

#include <utility>
#include <limits>
#include "../exceptions.h"

Node::Node(const NodeId& id, int size):
    id_(id), size_(size), nClones_(new int(0))
{
}

Node::Node(const tinyxml2::XMLElement *node):
    nClones_(new int(0))
{
    load(node);
}

void Node::addChild(NodePtr child)
{
    if(isValid() == true)
        throw InvalidArgumentException("Node::addChild: Cannot add child");

    children_.push_back( std::move(child) );
}

void Node::setChild(int i, NodePtr child)
{
    if(isValid() == false)
        throw InvalidArgumentException("Node::setChild: Cannot set child");

    if(i > (size_ - 1) && i < 0)
        throw InvalidArgumentException("Node::setChild: Cannot set child");

    children_[i] = std::move(child);
}

Node *Node::getChild(int i) const
{
    if(isValid() == false)
        throw InvalidArgumentException("Node::getChild: Cannot get child");

    if( (i > (size_ - 1)) && (i < 0) )
        throw InvalidArgumentException("Node::getChild: Cannot get child");

    return children_[i].get();
}

NodePtr Node::cloneChild(int i)
{
    if(isValid() == false)
        throw InvalidArgumentException("Node::getChild: Cannot return child");

    if( (i > (size_ - 1)) && (i < 0) )
        throw InvalidArgumentException("Node::getChild: Cannot return child");

    return children_[i]->clone();
}

void Node::giveChildren(Node &rhs)
{
    int lowerSize = size_ < rhs.size_ ? size_ : rhs.size_;
    int i = 0;
    for(;i < lowerSize; ++i)
        children_.push_back(std::move(rhs.children_[i]));
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

std::string Node::write() const
{
    std::string nodeString;
    nodeString += id_.toString() + " ";
    nodeString += "| ";
    nodeString += std::to_string(size_) + " ";

    writeNode(nodeString);

    return nodeString;
}

tinyxml2::XMLElement *Node::save(tinyxml2::XMLDocument &doc) const
{
    tinyxml2::XMLElement* node = doc.NewElement("Node");
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

void Node::load(const tinyxml2::XMLElement *node)
{
    id_.loadAttribute(node);
    size_ = node->IntAttribute("size");
}
