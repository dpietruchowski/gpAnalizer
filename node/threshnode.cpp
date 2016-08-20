#include "threshnode.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

using namespace std;
using namespace cv;
using namespace tinyxml2;

int ThreshNode::thresholdOperation(const std::vector<cv::Mat> &src,
                                   cv::Mat & dst,
                                   const ThreshParameters &param)
{
    if (src.size() != 1)
       {
           std::string exception = "Zla liczba argumentow";
           throw exception;
       }

   if((param.threshType > 4) || (param.threshType < 0))
       throw std::string("ThreshNode::thresholdOperation Bad thresh type");

   const cv::Mat& src1 = src[0];
   threshold(src1, dst, param.threshValue, 255, param.threshType);
   return 1;
}

NodePtr ThreshNode::create(unsigned int geneNumber)
{
    NodeId id = {THRESH_NODE, geneNumber, 0};
    ThreshParameters param = ThreshParameters::getRandom();

    return NodePtr( new ThreshNode(id, ThreshNode::thresholdOperation,
                                   param) );
}

NodePtr ThreshNode::createFromXml(const XMLElement *node)
{
    ThreshParameters param;
    param.loadAttribute(node);

    return NodePtr( new ThreshNode(node, ThreshNode::thresholdOperation,
                                   param) );
}

void ThreshNode::execute(const std::vector<Mat> &src, Mat &dst) const
{
    threshOperation_(src, dst, parameters_);
}

void ThreshNode::mutate()
{
    parameters_ = ThreshParameters::getRandom();
}

ThreshNode::ThreshNode(const NodeId &id, ThreshNode::ThreshPtr threshOperation,
                       const ThreshParameters &param):
    Node(id, 1), threshOperation_(threshOperation), parameters_(param)
{
}

ThreshNode::ThreshNode(const tinyxml2::XMLElement *node,
                       ThreshNode::ThreshPtr threshOperation,
                       const ThreshParameters &param):
    Node(node), threshOperation_(threshOperation), parameters_(param)
{
}

ThreshNode::ThreshNode(const ThreshNode &rhs):
    Node(rhs), threshOperation_(rhs.threshOperation_),
    parameters_(rhs.parameters_)
{
}

void ThreshNode::writeNode(string &nodeString) const
{
    nodeString += parameters_.toString();
}

NodePtr ThreshNode::cloneNode() const
{
    NodePtr cloned( new ThreshNode(*this) );
    return cloned;
}

void ThreshNode::save(XMLDocument &doc, XMLElement *node) const
{
    XMLElement *param = parameters_.save(doc);
    node->InsertEndChild(param);
}

void ThreshNode::save(XMLElement *node) const
{
    parameters_.saveAttribute(node);
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Thresh Parameter //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ThreshParameters ThreshParameters::getRandom()
{
    return ThreshParameters(1 + std::rand() % 254,
                            std::rand() % 5);
}

std::string ThreshParameters::typeToString() const
{
    switch(threshType)
    {
    case cv::THRESH_BINARY: return "ThreshBinary";
    case cv::THRESH_BINARY_INV: return "ThreshBinaryInv";
    case cv::THRESH_TOZERO: return "ThreshToZero";
    case cv::THRESH_TOZERO_INV: return "ThreshToZeroInv";
    case cv::THRESH_TRUNC: return "ThreshTrunc";
    default:
        return "Undefined";
    }
}

int ThreshParameters::typeFromString(const std::string &type) const
{
    if(type == "ThreshBinary") return cv::THRESH_BINARY;
    if(type == "ThreshBinaryInv") return cv::THRESH_BINARY_INV;
    if(type == "ThreshToZero") return cv::THRESH_TOZERO;
    if(type == "ThreshToZeroInv") return cv::THRESH_TOZERO_INV;
    if(type == "ThreshTrunc") return cv::THRESH_TRUNC;

    throw string("ThreshParameters::typeFromString: zly typ");
}

std::string ThreshParameters::toString() const
{
    std::string param;
    param += typeToString();
    param += " ";
    param += std::to_string(threshValue);

    return param;
}

//Format: "ThreshTrunc 32"
void ThreshParameters::fromString(const std::string &param)
{
    size_t space = param.find_first_of(" ");
    string type = param.substr(0, space);
    threshType = typeFromString(type);
    string value = param.substr(space+1);
    threshValue = stoi(value);
}

XMLElement *ThreshParameters::save(XMLDocument &doc) const
{
    XMLElement *param = doc.NewElement("ThreshParam");
    saveAttribute(param);

    return param;
}

void ThreshParameters::saveAttribute(XMLElement *node) const
{
    node->SetAttribute("value", threshValue);
    node->SetAttribute("threshType", typeToString().c_str());
}

void ThreshParameters::loadAttribute(const XMLElement *node)
{
    threshValue = node->IntAttribute("value");
    const char* type = node->Attribute("threshType");
    threshType = typeFromString(string(type));
}
