#include "funcnode.h"

using namespace std;

funcNode::funcNode(pair<funcId,func> p, string id):
    node(id, FUNCTION_NODE)
{
    this->argumentsNumber = p.first.first;
    this->functionName = p.first.second;
    this->function = p.second;
}

funcNode::funcNode(pair<funcId,func> p):
    node("pxtynz", FUNCTION_NODE)
{
    this->argumentsNumber = p.first.first;
    this->functionName = p.first.second;
    this->function = p.second;
}

string funcNode::getInfo()
{
    string info = this->functionName +
            " " + to_string(this->argumentsNumber);

    return info;
}


void funcNode::showNode()
{

}

node_ptr funcNode::copyAttribute()
{
    pair<funcId,func> p;
    funcId id = make_pair
            (this->argumentsNumber, this->functionName);
    p = make_pair(id, this->function);
    node_ptr newNode(new funcNode(p));

    return newNode;
}

void funcNode::execute(const vector<Mat> &src, Mat &dst)
{
    this->function(src, dst);
}

int funcNode::getArgumentsNumber()
{
    return argumentsNumber;
}
