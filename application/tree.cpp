#include "tree.h"

using namespace std;

tree::tree(int depth, string id)
{
    counterNodes = 0;
    this->depth = depth;
    this->id = id;
}

tree::tree(int depth)
{
    counterNodes = 0;
    this->depth = depth;
    this->id = "pxty";
}

tree::~tree()
{
}

void tree::initializeRoot(const functionSet &functions, const terminalSet &terminals)
{
    if (this->depth <= 1)
    {
        string id = this->id + 'n' + to_string(counterNodes++);
        this->root = node_ptr( new termNode(terminals.getRandomImage(), id) );
    } else
    {
        this->root = move( getRandomNode(functions) );
    }
}

void tree::setId(string id)
{
    this->id = id;
    this->counterNodes = 0;
    node* root = this->root.get();

    id = this->id + 'n' + to_string(counterNodes++);
    root->setId(id);
    setSubtreeId(root);
}

void tree::setSubtreeId(node *subroot)
{
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        string id = this->id + 'n' + to_string(counterNodes++);
        subroot->getChild(i)->setId(id);
    }
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        setSubtreeId(subroot->getChild(i));
    }
}

void tree::setRoot(node_ptr root)
{
    string id = this->id + 'n' + to_string(0);
    this->root = move(root);
    this->root->setId(id);
    checkDepth(this->root.get(), 1);
}

node* tree::getRoot() const
{
    return this->root.get();
}

node_ptr tree::returnRoot()
{
    return move(this->root);
}

node* tree::getNode(int i) const
{
    if (i == 0)
        return this->root.get();
    return getNode(this->root.get(), i-1);
}

node* tree::getNode(node* subroot, int n) const
{
    node* retNode = subroot;
    int childrenSize = subroot->getChildrenSize();
    if (n < childrenSize)
    {
        retNode = subroot->getChild(n);
    } else
    {
        for (int i = 0; i < childrenSize; i++)
        {
            node* child = subroot->getChild(i);
            int subtreeSize = this->getSubtreeSize(child);
            if ( n  < subtreeSize + 1 )
            {
                retNode = getNode(child, n-childrenSize);
                break;
            }
            else
                n -= (subtreeSize - 1);
        }
    }

    return retNode;
}

void tree::setSubtree(int i, node_ptr newNode)
{
    if (i > (this->getSize() - 1))
    {
        string exception = "Numer wezla wiekszy od liczby wezlow w drzewie";
        throw exception;
    }
    if (i == 0)
        setRoot( move(newNode) );
    setSubtree(this->root.get(), i-1, move(newNode));
}

void tree::setSubtree(node* subroot, int n, node_ptr newNode)
{
    int childrenSize = subroot->getChildrenSize();
    if (n < childrenSize)
    {
        subroot->setChild(n, move(newNode));
    } else
    {
        for (int i = 0; i < childrenSize; i++)
        {
            node* child = subroot->getChild(i);
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

string tree::getId() const
{
    return this->id;
}

int tree::getDepth() const
{
    return this->depth;
}

int tree::getSize() const
{
    return getSubtreeSize(this->root.get());
}

int tree::getSubtreeSize(node* subroot) const
{
    int size = 1;
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        size += getSubtreeSize(subroot->getChild(i));
    }

    return size;
}

void tree::initialize(typeInit type, const functionSet &functions,
                      const terminalSet &terminals)
{
    if (counterNodes > 1)
    {
        string exception = "Drzewo zostalo juz zainicjowane";
        throw exception;
    }

    initializeRoot(functions, terminals);

    if (type == GROW_INIT)
    {
        initializeGrow(functions, terminals, this->root.get(), this->depth-1);
        this->depth = 1;
        checkDepth(this->root.get(), this->depth);
            //Change depth, After grow init can be different
    } else if (type == FULL_INIT)
        initializeFull(functions, terminals, this->root.get(), this->depth-1);
}

int tree::initializeGrow(const functionSet& functions, const terminalSet& terminals,
                         node* subroot, int depth)
{
    if (subroot == nullptr)
    {
        string exception = "Nie istnieje taki wezel";
        throw exception;
    }
    int isEnd = 0;
    if (depth < (this->depth - (1 + rand() % 2))) isEnd = rand() % 2;
        //Choose randomly if children of subroot should be terminals.
    if (depth <= 1 || isEnd)
    {
        for (int i = 0; i < subroot->getArgumentsNumber(); i++)
        {
            string id = this->id + 'n' + to_string(counterNodes++);
            subroot->addChild(node_ptr( new termNode(terminals.getRandomImage(), id) ));
        }
        return 1;
    } else
    {
        for (int i = 0; i < subroot->getArgumentsNumber(); i++)
        {
            subroot->addChild(move( this->getRandomNode(functions) ));
        }
    }

    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        initializeGrow(functions, terminals, subroot->getChild(i), depth-1);
    }

    return 0;
}

int tree::initializeFull(const functionSet& functions, const terminalSet& terminals,
                         node* subroot, int depth)
{
    if (subroot == nullptr)
    {
        string exception = "Nie istnieje taki wezel";
        throw exception;
    }
    if (depth <= 1)
    {
        for (int i = 0; i < subroot->getArgumentsNumber(); i++)
        {
            string id = this->id + 'n' + to_string(counterNodes++);
            subroot->addChild(node_ptr( new termNode(terminals.getRandomImage(), id) ));
        }

        return 1;
    } else
    {
        for (int i = 0; i < subroot->getArgumentsNumber(); i++)
        {
            subroot->addChild(move( this->getRandomNode(functions) ));
        }
    }

    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        initializeFull(functions, terminals, subroot->getChild(i), depth-1);
    }

    return 0;
}

Mat tree::run()
{
    return runSubtree(this->root.get());
}

Mat tree::runSubtree(node* subroot)
{
    if (subroot == nullptr)
    {
        string exception = "Nie istnieje taki wezel";
        throw exception;
    }
    Mat result; // the result of function
    vector<Mat> arg; // arguments of function

    for (int i = 0; i < subroot->getArgumentsNumber(); i++)
    {
        arg.push_back( runSubtree(subroot->getChild(i)) );
    }

    subroot->execute(arg, result);

    return result;
}

tree_ptr tree::copy(string newId)
{
    return move( this->copySubtree(0, newId) );
}

tree_ptr tree::copySubtree(int subrootI, string newId)
{
    if (subrootI > (this->getSize() - 1))
    {
        string exception = "Nie ma wezla o takim numerze";
        throw exception;
    }
    node* subroot = this->getNode(subrootI);
    node_ptr newRoot( subroot->copyAttribute() );

    copySubtree(subroot, newRoot.get());

    int newDepth = getSubtreeDepth(subrootI);
    tree_ptr newTree(new tree(newDepth));
    newTree->setRoot( move(newRoot) );
    newTree->setId(newId);

    return newTree;
}

tree_ptr tree::copy()
{
    return move( this->copySubtree(0) );
}

tree_ptr tree::copySubtree(int subrootI)
{
    if (subrootI > (this->getSize() - 1))
    {
        string exception = "Nie ma wezla o takim numerze";
        throw exception;
    }
    node* subroot = this->getNode(subrootI);
    node_ptr newRoot( subroot->copyAttribute() );

    copySubtree(subroot, newRoot.get());

    int newDepth = getSubtreeDepth(subrootI);
    tree_ptr newTree(new tree(newDepth));
    newTree->setRoot( move(newRoot) );

    return newTree;
}

int tree::copySubtree(node* subroot, node* newSubroot)
{
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        node_ptr newNode( subroot->getChild(i)->copyAttribute() );
        newSubroot->addChild( move(newNode) );
    }

    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        copySubtree(subroot->getChild(i), newSubroot->getChild(i));
    }

    return 0;
}

int tree::getSubtreeDepth(int subrootI) const
{
    if (subrootI > (this->getSize() - 1))
    {
        string exception = "Nie ma wezla o takim numerze";
        throw exception;
    }

    return getSubtreeDepth(this->getNode(subrootI), 1);
}

int tree::getSubtreeDepth(node* subroot, int depth) const
{
    int maxDepth = 0;
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        maxDepth = getSubtreeDepth(subroot->getChild(i), depth+1);
    }
    if (depth > maxDepth)
        maxDepth = depth;

    return maxDepth;
}

node_ptr tree::getRandomNode(const functionSet &functions)
{
    int random = rand() % 3;
    node_ptr randomNode = nullptr;
    if (random == 0)
    {
        string id = this->id + 'n' + to_string(counterNodes++);
        randomNode = node_ptr( new funcNode(functions.getRandomFunction(), id) );
    } else
    {

        string id = this->id + 'n' + to_string(counterNodes++);
        randomNode = node_ptr( new morphoNode(functions.getRandomOperation(), id,
                                    functions.getRandomMorphParameters()) );
    }
    return randomNode;
}

void tree::show()
{
    cout << "::::::::::: treeId: " << this->id << " ::::::::::::" << endl;
    showSubtree(this->root.get(), "");
    cout<<endl;
}

void tree::showSubtree(node *subroot, string cou)
{
    cout << cou;
    subroot->show();
    cou+="----->";
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        showSubtree(subroot->getChild(i), cou);
    }
}

void tree::checkDepth(node *subroot, int depth)
{
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        checkDepth(subroot->getChild(i), depth+1);
    }
    if (depth > this->depth)
        this->depth = depth;
}

string tree::writeTree()
{
    string treeString = "::::::::::: treeId: "
            + this->id +
            " ::::::::::::\n";
    string arrows = "";
    treeString += writeSubtree(this->root.get(), arrows);

    return treeString;
}

string tree::writeSubtree(node *subroot, string arrows)
{
    string subtreeString = arrows + subroot->writeNode();
    arrows += "------->";
    for (int i = 0; i < subroot->getChildrenSize(); i++)
    {
        subtreeString += writeSubtree(subroot->getChild(i), arrows);
    }
    return subtreeString;
}
