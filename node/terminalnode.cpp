#include "terminalnode.h"

#include <opencv2/highgui/highgui.hpp>

using namespace std;

TerminalSet TerminalNode::terminals_ = TerminalSet(MatPtr(new cv::Mat
                                                  (cv::imread("null.png", 0))));

TerminalSet &TerminalNode::getTerminalSet()
{
    return terminals_;
}

NodePtr TerminalNode::create(unsigned int geneNumber)
{
    NodeId id = {TERMINAL_NODE, geneNumber, 0};
    return NodePtr(new TerminalNode(id, terminals_.getTerminal()));
}

void TerminalNode::execute(const std::vector<cv::Mat> &, cv::Mat &dst) const
{
    if(terminal_->empty())
        throw string("Nie wczytano obrazka");
    if(terminal_->type() != CV_8U)
        throw string("Obrazek niepoprawny");

    terminal_->copyTo(dst);
}

TerminalNode::TerminalNode(const NodeId &id, MatPtr terminal):
    Node(id, 0), terminal_(terminal)
{
}

TerminalNode::TerminalNode(const TerminalNode &rhs):
    Node(rhs), terminal_(rhs.terminal_)
{
}

void TerminalNode::writeNode(string &nodeString) const
{
    nodeString += "input";
}

NodePtr TerminalNode::cloneNode() const
{
    NodePtr cloned( new TerminalNode(*this) );
    return cloned;
}

