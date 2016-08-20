#ifndef TREE_H
#define TREE_H

#include "node/node.h"
#include "generator/nodegenerator.h"
#include "parser/parser.h"
#include <tinyxml2.h>

class Tree;
typedef std::unique_ptr<Tree> TreePtr;

enum InitType
{
    GROW_INIT,
    FULL_INIT
};

struct NodeParent
{
    Node* parent;
    int childNumber;
};

class Tree
{
public:
    Tree(int depth, int id);
    ~Tree() {}
public:
    void initialize(InitType type, NodeGenerator& generator);
    cv::Mat run() const;
    TreePtr clone(int newId) const;
    TreePtr cloneSubtree(int subrootI, int newId) const;

public:
    void setNode(int nodeNumber, const Node& node);
    void setSubtree(int nodeNumber, const Node& root);

    int getId() const;
    int getSize() const;
    int getDepth() const;
    int getSubtreeDepth(int subrootI) const;
    std::string write() const;
    tinyxml2::XMLElement* save(tinyxml2::XMLDocument& doc) const;
    void parse(tinyxml2::XMLElement* tree, Parser& parser);

    Node* getRoot() const;
    Node* getNode(int i) const;

private:
    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////// Initialize functions /////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    int initialize(NodeGenerator& generator, Node* subroot, bool terminal);
    void initializeRoot(NodeGenerator& generator);
    int initializeGrow(NodeGenerator& generator, Node* subroot, int depth);
    int initializeFull(NodeGenerator& generator, Node* subroot, int depth);

    int getSubtreeSize(Node* subroot) const;
    int getSubtreeDepth(Node* subroot, int depth) const;
    cv::Mat runSubtree(Node* subroot) const;
    void checkDepth(Node *subroot, int depth);
    NodeParent getNodeParent(Node* subroot, int n) const;
    void setSubtree(Node* subroot, int n, NodePtr newNode);
    std::string writeSubtree(Node *subroot, std::string arrows) const;
    void saveSubtree(tinyxml2::XMLDocument &doc, Node *subroot,
                     tinyxml2::XMLElement* subrootXml) const;
    void parseSubtree(Node* subroot, tinyxml2::XMLElement* subrootXml,
                      Parser& parser);

    ////////////////////////////////////////////////////////////////////////////
    //////////////////////////// Clone functions ///////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    void clone(const Tree& rhs);
    void clone(const Node& rhs);
    int cloneSubtree(const Node* subroot, Node* newSubroot);

private:
    int id_;
    int depth_;
    int counterNodes_;
    NodePtr root_;

    std::shared_ptr<int> nClones;
};

#endif // TREE_H
