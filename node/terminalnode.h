#ifndef TERMINALNODE_H
#define TERMINALNODE_H

#include "node.h"

class TerminalNode : public Node
{
public:
    static NodePtr create(unsigned int geneNumber);
    static NodePtr createFromXml(const tinyxml2::XMLElement *node);

public:
    void execute(const std::vector<cv::Mat>&, cv::Mat&) const {}
    void mutate() {}

private:
    TerminalNode(const NodeId& id);
    TerminalNode(const tinyxml2::XMLElement *node);
    TerminalNode(const TerminalNode& rhs);

private:
    virtual void writeNode(std::string& nodeString) const;
    virtual NodePtr cloneNode() const;
    virtual void save(tinyxml2::XMLDocument&,
                      tinyxml2::XMLElement *) const {}
    virtual void save(tinyxml2::XMLElement *) const {}
};

#endif // TERMINALNODE_H
