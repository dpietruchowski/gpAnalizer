#include "morphonode.h"

using namespace std;

morphoNode::morphoNode(pair<funcId,morpho> p, string id, morphologyParameters morph):
    node(id, MORPHOLOGY_NODE)
{
    this->argumentsNumber = p.first.first;
    this->operationName = p.first.second;
    this->operation = p.second;
    this->morph.size1 = morph.size1;
    this->morph.size2 = morph.size2;
    this->morph.elementType = morph.elementType;
    this->morph.morphType = morph.morphType;
    this->morph.iteration = morph.iteration;
}

morphoNode::morphoNode(pair<funcId,morpho> p, morphologyParameters morph):
    node("pxtynz", MORPHOLOGY_NODE)
{
    this->argumentsNumber = p.first.first;
    this->operationName = p.first.second;
    this->operation = p.second;
    this->morph.size1 = morph.size1;
    this->morph.size2 = morph.size2;
    this->morph.elementType = morph.elementType;
    this->morph.morphType = morph.morphType;
    this->morph.iteration = morph.iteration;
}

string morphoNode::getInfo()
{
    string info = this->operationName +
            " " + to_string(this->argumentsNumber) +
            " " + to_string(morph.size1) + " " +
            to_string(morph.size2) + " " +
            to_string(morph.morphType) + " " +
            to_string(morph.elementType) + " " +
            to_string(morph.iteration);

    return info;
}

void morphoNode::showNode()
{

}

node_ptr morphoNode::copyAttribute()
{
    pair<funcId,morpho> p;
    funcId id = make_pair
            (this->argumentsNumber, this->operationName);
    p = make_pair(id, this->operation);
    node_ptr newNode(new morphoNode(p, this->morph));

    return newNode;
}

void morphoNode::execute(const vector<Mat> &src, Mat &dst)
{
    this->operation(src, dst, morph);
}

int morphoNode::getArgumentsNumber()
{
    return argumentsNumber;
}
