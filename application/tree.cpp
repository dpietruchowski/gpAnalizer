#include "tree.h"

using namespace std;
using namespace cv;
using namespace tinyxml2;

Tree::Tree(int depth, int id):
    id_(id), depth_(depth), counterNodes_(0), nClones(new int(0))
{
}

void Tree::initialize(InitType type, NodeGenerator &generator)
{
//    if (counterNodes_ > 1)
//    {
//       string exception = "Drzewo zostalo juz zainicjowane";
//       throw exception;
//    }

    initializeRoot(generator);

    if (type == GROW_INIT)
    {
       initializeGrow(generator, root_.get(), depth_-1);
       depth_ = getSubtreeDepth(0);
           //Change depth, After grow init can be different
    } else if (type == FULL_INIT)
        initializeFull(generator, root_.get(), depth_-1);

    Mat result = run();
    threshold(result, result, 125, 255, 0);
    int whitePixels = countNonZero(result);
    int blackPixels = result.total() - whitePixels;

    if( (whitePixels == 0) || (blackPixels == 0) )
        initialize(type, generator);
}

cv::Mat Tree::run() const
{
    return runSubtree(root_.get());
}

TreePtr Tree::clone(int newId) const
{
    return cloneSubtree(0, newId);
}

TreePtr Tree::cloneSubtree(int subrootI, int newId) const
{
    if (subrootI > (getSize() - 1))
    {
        string exception = "Nie ma wezla o takim numerze";
        throw exception;
    }

    int newDepth = getSubtreeDepth(subrootI);
    TreePtr newTree(new Tree(newDepth, newId));
    Node* subroot = getNode(subrootI);
    newTree->clone(*subroot);
    newTree->depth_ = newTree->getSubtreeDepth(0);

    return newTree;
}

void Tree::setNode(int nodeNumber, const Node &node)
{
    NodeParent nodeParent;
    NodePtr newNode = node.clone();
    Node* replacedNode = nullptr;
    if(nodeNumber == 0)
        replacedNode = root_.get();
    else
    {
        nodeParent = getNodeParent(root_.get(), nodeNumber-1);
        replacedNode = nodeParent.parent->getChild(nodeParent.childNumber);
    }

    newNode->giveChildren(*replacedNode);

    if(nodeNumber == 0)
        root_ = move(newNode);
    else
    {
        nodeParent.parent->setChild(nodeParent.childNumber, move(newNode));
    }

    depth_ = getSubtreeDepth(0);
}

void Tree::setSubtree(int nodeNumber, const Node &root)
{
    NodePtr newNode = root.clone();
    cloneSubtree(&root, newNode.get());

    if(nodeNumber == 0)
        root_ = move(newNode);
    else
    {
        NodeParent nodeParent = getNodeParent(root_.get(), nodeNumber-1);
        nodeParent.parent->setChild(nodeParent.childNumber, move(newNode));
    }

    depth_ = getSubtreeDepth(0);
}

int Tree::getId() const
{
    return id_;
}

int Tree::getSize() const
{
    return getSubtreeSize(root_.get());
}

int Tree::getSubtreeSize(Node* subroot) const
{
    int size = 1;
    for (int i = 0; i < subroot->getSize(); i++)
    {
        size += getSubtreeSize(subroot->getChild(i));
    }

    return size;
}

int Tree::getDepth() const
{
    return depth_;
}

int Tree::getSubtreeDepth(int subrootI) const
{
    if (subrootI > (this->getSize() - 1))
    {
        string exception = "Nie ma wezla o takim numerze";
        throw exception;
    }

    return getSubtreeDepth(getNode(subrootI), 0);
}

string Tree::write() const
{
    string treeString = "::::::::::: treeId: "
                + to_string(id_) + " ::::::::::::\n";
    string arrows = "";
    treeString += writeSubtree(root_.get(), arrows);

    return treeString;
}

XMLElement *Tree::save(XMLDocument &doc) const
{
    XMLElement* tree = doc.NewElement("Tree");
    tree->SetAttribute("id", id_);
    tree->SetAttribute("depth", depth_);
    tree->SetAttribute("size", getSize());

    XMLElement *root = root_->save(doc);
    tree->InsertFirstChild(root);

    saveSubtree(doc, root_.get(), root);

    return tree;
}

void Tree::parse(XMLElement *tree,
                 Parser& parser)
{
    id_ = tree->IntAttribute("id");
    depth_ = tree->IntAttribute("depth");

    XMLElement *root = tree->FirstChildElement();
    root_ = move( parser.parseNode(root) );

    parseSubtree(root_.get(), root, parser);
}

int Tree::getSubtreeDepth(Node* subroot, int depth) const
{
    int maxDepth = 0;
    for (int i = 0; i < subroot->getSize(); i++)
    {
        maxDepth = getSubtreeDepth(subroot->getChild(i), depth+1);
    }
    if (depth > maxDepth)
        maxDepth = depth;

    return maxDepth;
}

Node *Tree::getRoot() const
{
    return root_.get();
}

Node *Tree::getNode(int i) const
{
    if (i == 0)
        return root_.get();

    NodeParent nodeParent = getNodeParent(root_.get(), i-1);
    return nodeParent.parent->getChild(nodeParent.childNumber);
}

int Tree::initialize(NodeGenerator &generator, Node *subroot, bool terminal)
{
    if (subroot == nullptr)
        throw string("Nie istnieje taki wezel");

    if (terminal == true)
    {
        for (int i = 0; i < subroot->getSize(); i++)
        {
            subroot->addChild( TerminalNode::create(0) );
            counterNodes_++;
        }

        return 1;
    } else
    {
        for (int i = 0; i < subroot->getSize(); i++)
        {
            subroot->addChild( generator.createRandomPtr() );
            counterNodes_++;
        }
    }

    return 0;
}

void Tree::initializeRoot(NodeGenerator &generator)
{
    if (depth_ <= 0)
        root_ = move (NodePtr( TerminalNode::create(0) ));
    else
        root_ = move( generator.createRandomPtr() );
    counterNodes_++;
}

int Tree::initializeGrow(NodeGenerator &generator, Node *subroot, int depth)
{
    int isEnd = 0;
    if (depth < (depth_ - (1 + rand() % 2))) isEnd = rand() % 2;
        //Choose randomly if children of subroot should be terminals.
    initialize(generator, subroot, (depth <= 1 || isEnd));

    for (int i = 0; i < subroot->getSize(); i++)
    {
        initializeGrow(generator, subroot->getChild(i), depth-1);
    }

    return 0;
}

int Tree::initializeFull(NodeGenerator &generator, Node *subroot, int depth)
{
    initialize(generator, subroot, (depth <= 0));

    for (int i = 0; i < subroot->getSize(); i++)
    {
        initializeFull(generator, subroot->getChild(i), depth-1);
    }

    return 0;
}

NodeParent Tree::getNodeParent(Node *subroot, int n) const
{
    NodeParent retNodeParent;
    retNodeParent.parent = subroot;
    retNodeParent.childNumber = n;
    int childrenSize = subroot->getSize();
    if (n >= childrenSize)
    {
        for (int i = 0; i < childrenSize; i++)
        {
            Node* child = subroot->getChild(i);
            int subtreeSize = getSubtreeSize(child);
            if ( n  < subtreeSize + 1 )
            {
                retNodeParent = getNodeParent(child, n-childrenSize);
                break;
            }
            else
                n -= (subtreeSize - 1);
        }
    }

    return retNodeParent;
}

void Tree::setSubtree(Node *subroot, int n, NodePtr newNode)
{
    int childrenSize = subroot->getSize();
    if (n < childrenSize)
    {
        subroot->setChild(n, move(newNode));
    } else
    {
        for (int i = 0; i < childrenSize; i++)
        {
            Node* child = subroot->getChild(i);
            int subtreeSize = this->getSubtreeSize(child);
            if ( n  < subtreeSize + 1 )
            {
                setSubtree(child, n-childrenSize, move(newNode));
                break;
            }
            else
                n -= (subtreeSize - 1);
        }
    }
}

string Tree::writeSubtree(Node *subroot, string arrows) const
{
    string subtreeString = arrows + subroot->write();
    subtreeString += "\n";
    arrows += "------->";
    for (int i = 0; i < subroot->getSize(); i++)
    {
        subtreeString += writeSubtree(subroot->getChild(i), arrows);
    }

    return subtreeString;
}

void Tree::saveSubtree(XMLDocument &doc, Node *subroot,
                       XMLElement *subrootXml) const
{
    for(int i = 0; i < subroot->getSize(); ++i)
    {
        Node *child = subroot->getChild(i);
        XMLElement *childXml = child->save(doc);
        subrootXml->InsertEndChild( childXml );
        saveSubtree(doc, child, childXml);
    }
}

void Tree::parseSubtree(Node *subroot, XMLElement *subrootXml, Parser &parser)
{
    XMLElement *childXml = subrootXml->FirstChildElement();
    for(int i = 0; i < subroot->getSize(); ++i)
    {
        NodePtr child = parser.parseNode(childXml);
        Node *node = child.get();
        subroot->addChild( move(child) );
        parseSubtree(node, childXml, parser);

        childXml = childXml->NextSiblingElement();
    }
}

void Tree::clone(const Tree &rhs)
{
    root_ = rhs.getRoot()->clone();
    cloneSubtree(rhs.getRoot(), root_.get());
}

void Tree::clone(const Node &rhs)
{
    root_ = rhs.clone();
    cloneSubtree(&rhs, root_.get());
}

int Tree::cloneSubtree(const Node *subroot, Node *newSubroot)
{
    for (int i = 0; i < subroot->getSize(); i++)
    {
        newSubroot->addChild( subroot->getChild(i)->clone() );
    }

    for (int i = 0; i < subroot->getSize(); i++)
    {
        cloneSubtree(subroot->getChild(i), newSubroot->getChild(i));
    }

    return 0;
}

cv::Mat Tree::runSubtree(Node *subroot) const
{
    if (subroot == nullptr)
        throw string("Nie istnieje taki wezel");

    Mat result; // the result of function
    vector<Mat> arg; // arguments of function

    for (int i = 0; i < subroot->getSize(); i++)
    {
        arg.push_back( runSubtree(subroot->getChild(i)) );
    }

    subroot->execute(arg, result);

    return result;
}

void Tree::checkDepth(Node *subroot, int depth)
{
    for (int i = 0; i < subroot->getSize(); i++)
    {
        checkDepth(subroot->getChild(i), depth+1);
    }
    if (depth > depth_)
    depth_ = depth;
}
