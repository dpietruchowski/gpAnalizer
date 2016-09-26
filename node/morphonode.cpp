#include "morphonode.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <iterator>

using namespace std;
using namespace cv;
using namespace tinyxml2;

int MorphoNode::morphologyOperation(const std::vector<cv::Mat> &src, cv::Mat &dst,
                                    const MorphoParameters& param,
                                    const cv::Mat& element)
{
    if (src.size() != 1)
    {
        std::string exception = "Zla liczba argumentow";
        throw exception;
    }

//    if((param.morphShape > 2) || (param.morphShape < 0))
//        throw std::string("MorphoGene::morphologyOperation: Bad shape");
    if((param.morphType > 7) || (param.morphType < 0))
        throw std::string("MorphoGene::morphologyOperation: Bad morpho type");

    const cv::Mat& src1 = src[0];/*
    cv::Point anchor = cv::Point(10,10);
    cv::Mat element = cv::getStructuringElement(param.morphShape,
                                                cv::Size(param.morphWidth,
                                                     param.morphHeight),
                                                cv::Point(-1,-1));*/
    cv::morphologyEx(src1, dst, param.morphType, element,
                     cv::Point(-1,-1), param.iterations);
    return 1;
}

NodePtr MorphoNode::create(unsigned int geneNumber)
{
    NodeId id = {MORPHO_NODE, geneNumber, 0};
    MorphoElement element = MorphoElement::getRandom();
    MorphoParameters param = MorphoParameters::getRandom();

    return NodePtr( new MorphoNode(id, MorphoNode::morphologyOperation, param,
                                   element) );
}

NodePtr MorphoNode::createFromXml(const XMLElement *node)
{
    MorphoElement element;
    MorphoParameters param;
    element.loadAttribute(node);
    param.loadAttribute(node);

    return NodePtr( new MorphoNode(node, MorphoNode::morphologyOperation, param,
                                   element) );
}

void MorphoNode::execute(const std::vector<Mat> &src, Mat &dst) const
{
    morphoOperation_(src, dst, parameters_, structElement_.element);
}

void MorphoNode::mutate()
{
    parameters_ = MorphoParameters::getRandom();
    structElement_ = MorphoElement::getRandom();
}

MorphoNode::MorphoNode(const NodeId &id, MorphoNode::MorphoPtr morphoOperation,
                       const MorphoParameters & param, const MorphoElement &element):
    Node(id, 1), morphoOperation_(morphoOperation), parameters_(param),
    structElement_(element)
{
}

MorphoNode::MorphoNode(const XMLElement *node,
                       MorphoNode::MorphoPtr morphoOperation,
                       const MorphoParameters &param,
                       const MorphoElement &element):
    Node(node), morphoOperation_(morphoOperation), parameters_(param),
    structElement_(element)
{
}

MorphoNode::MorphoNode(const MorphoNode &rhs):
    Node(rhs), morphoOperation_(rhs.morphoOperation_), parameters_(rhs.parameters_),
        structElement_(rhs.structElement_)
{
}

void MorphoNode::writeNode(string &nodeString) const
{
    nodeString += parameters_.toString();
    nodeString += " Size(";
    nodeString += to_string(structElement_.element.rows);
    nodeString += ",";
    nodeString += to_string(structElement_.element.cols);
    nodeString += ")";
}

NodePtr MorphoNode::cloneNode() const
{
    NodePtr cloned( new MorphoNode(*this) );
    return cloned;
}

void MorphoNode::save(XMLDocument &doc, XMLElement *node) const
{
    XMLElement *morphoParam = parameters_.save(doc);
    node->InsertEndChild(morphoParam);

    XMLElement *element = structElement_.save(doc);
    node->InsertEndChild(element);
}

void MorphoNode::save(XMLElement *node) const
{
    parameters_.saveAttribute(node);
    structElement_.saveAttribute(node);
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// MorphologyParameters //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
MorphoParameters MorphoParameters::getRandom()
{
    return MorphoParameters(std::rand() % 8,
                            1 + std::rand() % MAX_ITER);
}

std::string MorphoParameters::typeToString() const
{
    switch(morphType)
    {
    case cv::MORPH_ERODE: return "Erode";
    case cv::MORPH_DILATE: return "Dilate";
    case cv::MORPH_OPEN: return "Open";
    case cv::MORPH_CLOSE: return "Close";
    case cv::MORPH_GRADIENT: return "Gradient";
    case cv::MORPH_TOPHAT: return "Tophat";
    case cv::MORPH_BLACKHAT: return "Blackhat";
    case cv::MORPH_HITMISS: return "Hitmiss";
    default:
        return "Undefined";
    }
}

int MorphoParameters::typeFromString(const std::string &type) const
{
    if(type == "Erode") return cv::MORPH_ERODE;
    if(type == "Dilate") return cv::MORPH_DILATE;
    if(type == "Open") return cv::MORPH_OPEN;
    if(type == "Close") return cv::MORPH_CLOSE;
    if(type == "Gradient") return cv::MORPH_GRADIENT;
    if(type == "Tophat") return cv::MORPH_TOPHAT;
    if(type == "Blackhat") return cv::MORPH_BLACKHAT;
    if(type == "Hitmiss") return cv::MORPH_HITMISS;

    throw "MorphoParameters::typeFromString: Zly parametr";
}

std::string MorphoParameters::toString() const
{
    std::string param;
    param += typeToString();
//    param += " ";
//    param += shapeToString();
    param += " ";
    param += std::to_string(iterations);

    return param;
}

//Format: "Gradient 2"
void MorphoParameters::fromString(const std::string & param)
{
    size_t space = param.find_first_of(" ");
    string type = param.substr(0, space);
    morphType = typeFromString(type);
    string iter = param.substr(space+1);
    iterations = stoi(iter);
}

XMLElement *MorphoParameters::save(XMLDocument &doc) const
{
    XMLElement *param = doc.NewElement("MorphoParam");
    saveAttribute(param);

    return param;
}

void MorphoParameters::saveAttribute(XMLElement *node) const
{
    node->SetAttribute("morphType", typeToString().c_str());
    node->SetAttribute("iterations", iterations);
}

void MorphoParameters::loadAttribute(const XMLElement *node)
{
    const char* type = node->Attribute("morphType");
    morphType = typeFromString(string(type));
    iterations = node->IntAttribute("iterations");
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// MorphologyElement //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
MorphoElement MorphoElement::getRandom()
{
    MorphoElement element;
    int width = 1 + std::rand() % MorphoParameters::MAX_WIDTH;
    int height = 1 + std::rand() % MorphoParameters::MAX_HEIGHT;
    element.morphShape = rand() % 3;
//    element.element = cv::Mat(1 + std::rand() % MorphoParameters::MAX_WIDTH,
//                    1 + std::rand() % MorphoParameters::MAX_HEIGHT,
//                    CV_8U);
//    cv::randu(element.element, cv::Scalar(0), cv::Scalar(2));
    element.element = cv::getStructuringElement(element.morphShape,
                                                cv::Size(width, height));

    return element;
}

string MorphoElement::shapeToString() const
{
    switch(morphShape)
    {
    case cv::MORPH_RECT: return "Rect";
    case cv::MORPH_ELLIPSE: return "Ellipse";
    case cv::MORPH_CROSS: return "Cross";
    default:
        return "Undefined";
    }
}

int MorphoElement::shapeFromString(const string &shape)
{
    if(shape == "Rect") return cv::MORPH_RECT;
    if(shape == "Ellipse") return cv::MORPH_ELLIPSE;
    if(shape == "Cross") return cv::MORPH_CROSS;

    throw "MorphoElement::shapeFromString: Zly parametr";
}

string MorphoElement::toString() const
{
    vector<int> vec(element.ptr(), element.ptr() + element.total());
    stringstream ss;
    copy(vec.begin(), vec.end(), std::ostream_iterator<int>(ss));
    return ss.str();
}

void MorphoElement::fromString(const string &selement, int nRows, int nCols)
{
    if(selement.size() != nRows * nCols)
        throw std::string("MorphoElement::fromString: Wrong selement");
    element = cv::Mat(nRows, nCols, CV_8U);
    vector<int> vv(selement.begin(), selement.end());
    for_each(vv.begin(),vv.end(),[](int &a){a-=48;});
    int k = 0;
    for(int i=0; i<element.rows; ++i)
         for(int j=0; j<element.cols; ++j)
             element.at<unsigned char>(i, j) = vv[k++];
}

XMLElement *MorphoElement::save(XMLDocument &doc) const
{
    XMLElement *morphoElement = doc.NewElement("MorphoElement");
    saveAttribute(morphoElement);

    return morphoElement;
}

void MorphoElement::saveAttribute(XMLElement *node) const
{
    node->SetAttribute("width", element.cols);
    node->SetAttribute("height", element.rows);
    node->SetAttribute("shape", shapeToString().c_str());
}

void MorphoElement::loadAttribute(const XMLElement *node)
{
    int width = node->IntAttribute("width");
    int height = node->IntAttribute("height");
    const char* shape = node->Attribute("shape");
    morphShape = shapeFromString(string(shape));

    element = cv::getStructuringElement(morphShape, cv::Size(width, height));
}
