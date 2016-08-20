#ifndef TERMINALNODE_H
#define TERMINALNODE_H

#include "node.h"
#include "terminalset.h"

typedef std::shared_ptr<cv::Mat> MatPtr;

class TerminalNode : public Node
{
public:
    static TerminalSet& getTerminalSet();
    static NodePtr create(unsigned int geneNumber);
    static NodePtr createFromXml(const tinyxml2::XMLElement *node);

public:
    void execute(const std::vector<cv::Mat>&, cv::Mat& dst) const;
    void mutate() {}

private:
    TerminalNode(const NodeId& id, MatPtr terminal);
    TerminalNode(const tinyxml2::XMLElement *node, MatPtr terminal);
    TerminalNode(const TerminalNode& rhs);

private:
    virtual void writeNode(std::string& nodeString) const;
    virtual NodePtr cloneNode() const;
    virtual void save(tinyxml2::XMLDocument&,
                      tinyxml2::XMLElement *) const {}
    virtual void save(tinyxml2::XMLElement *) const {}

private:
    MatPtr terminal_;

    static TerminalSet terminals_;
};

#endif // TERMINALNODE_H
