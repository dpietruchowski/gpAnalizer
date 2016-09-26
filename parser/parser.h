#ifndef PARSER_H
#define PARSER_H

#include "node/nodes.h"
#include <tinyxml2.h>

class Tree;
typedef std::unique_ptr<Tree> TreePtr;

class Parser
{
public:
    Parser();
    NodePtr parseNode(const tinyxml2::XMLElement* node);
    TreePtr parseTree(tinyxml2::XMLElement *tree,  const cv::Mat& image);
    TreePtr parse(std::string fileName, const cv::Mat &image);
};

#endif // PARSER_H
