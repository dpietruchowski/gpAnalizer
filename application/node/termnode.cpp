#include "termnode.h"
#include "application/terminalset.h"

using namespace std;

termNode::termNode(Mat_ptr image, string id):
    node(id, TERMINAL_NODE), terminal(image)
{
    this->terminalName = "inputImg";
}

termNode::termNode(Mat_ptr image):
    node("pxtynz", TERMINAL_NODE), terminal(image)
{
    this->terminalName = "inputImg";
}

string termNode::getInfo()
{
    string info = this->terminalName;

    return info;
}

void termNode::showNode()
{

}

void termNode::execute(const std::vector<Mat> &src, Mat &dst)
{
    this->terminal->copyTo(dst);
}

int termNode::getArgumentsNumber()
{
    return 0;
}

node_ptr termNode::copyAttribute()
{
   node_ptr newNode(new termNode(this->terminal));

   return newNode;
}
