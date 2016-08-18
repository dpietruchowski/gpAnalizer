#include "node.h"

using namespace std;

node::node(string id, typeNode type)
{
    this->id=id;
    this->type=type;
}

int node::getChildrenSize() const
{
    return this->children.size();
}

node* node::getChild(int i) const
{
    size_t in(i);
    if (in > (children.size() - 1))
    {
        string exception = "Nie ma dziecka o takim numerze";
        throw exception;
    }
    return children[i].get();
}

void node::setChild(int i, node_ptr newChild)
{
    size_t in(i);
    if (in > (children.size() - 1))
    {
        string exception = "Nie ma dziecka o takim numerze";
        throw exception;
    }
    children[i] = move(newChild);
}

node_ptr node::returnChild(int i)
{
    size_t in(i);
    if (in > (children.size() - 1))
    {
        string exception = "Nie ma dziecka o takim numerze";
        throw exception;
    }
    node_ptr returnedChild = move(children[i]);
    children.erase(children.begin() + i);

    return returnedChild;
}

void node::addChild(node_ptr child)
{
    this->children.push_back( move(child) );
}

string node::getId() const
{
    return this->id;
}

typeNode node::getType() const
{
    return this->type;
}

void node::show()
{
}

void node::setId(string id)
{
    this->id=id;
}

string node::writeNode()
{
    string nodeString = this->id + " ";
    nodeString += this->getInfo();
    nodeString += "\n";

    return nodeString;
}
