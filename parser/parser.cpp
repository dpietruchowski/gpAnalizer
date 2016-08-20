#include "parser.h"
#include <string>
#include <tinyxml2.h>
#include "application/tree.h"

using namespace std;
using namespace tinyxml2;

Parser::Parser()
{

}

TreePtr Parser::parse(std::string fileName)
{
      XMLDocument doc;
      doc.LoadFile(fileName.c_str());
      XMLElement *tree = doc.FirstChildElement();

      return parseTree(tree);
}

NodePtr Parser::parseNode(const tinyxml2::XMLElement *node)
{
    const char* typeStr = node->Attribute("type");
    NodeType type = enumFromString(std::string(typeStr));
    switch(type)
    {
    case TERMINAL_NODE: return TerminalNode::createFromXml(node);
    case FUNCTION_NODE: return FunctionNode::createFromXml(node);
    case MORPHO_NODE: return MorphoNode::createFromXml(node);
    case THRESH_NODE: return ThreshNode::createFromXml(node);
    case NULL_NODE:
    default:
        throw std::string("Zły typ");
    }
}

TreePtr Parser::parseTree(tinyxml2::XMLElement *tree)
{
    int depth = tree->IntAttribute("depth");
    int id = tree->IntAttribute("id");
    TreePtr treePtr(new Tree(depth, id));
    treePtr->parse(tree, *this);

    return treePtr;
}

