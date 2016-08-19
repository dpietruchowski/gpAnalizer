#ifndef NODE_H
#define NODE_H

#include <memory>
#include <opencv2/core/core.hpp>
#include "nodeid.h"

class Node;
typedef std::unique_ptr<Node> NodePtr;

class Node
{
public:
    Node(const NodeId& id, int size);
    virtual ~Node() {}
    void addChild(NodePtr child);
    void setChild(int i, NodePtr child);
    Node* getChild(int i) const;
    NodePtr cloneChild(int i);
    void giveChildren(Node& rhs);

    int getSize() const;
    int getActualSize() const;
    bool isValid() const;
    const NodeId &getId() const;

    std::string write() const;
    NodePtr clone() const;

    virtual void execute(const std::vector<cv::Mat>& src, cv::Mat& dst) const = 0;
    virtual void mutate() = 0;

protected:
    Node(const Node& rhs);

private:
    virtual void writeNode(std::string& nodeString) const = 0;
    virtual NodePtr cloneNode() const = 0;

private:
    NodeId id_;
    int size_;
    std::vector<NodePtr> children_;

    std::shared_ptr<int> nClones_;
};

#endif // NODE_H
