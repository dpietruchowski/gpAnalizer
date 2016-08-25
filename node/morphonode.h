#ifndef MORPHONODE_H
#define MORPHONODE_H

#include "node.h"

struct MorphoParameters
{
    const static int MAX_WIDTH = 15;
    const static int MAX_HEIGHT = 15;
    const static int MAX_ITER = 10;
    static MorphoParameters getRandom();
    int morphType; // max 7
//    int morphShape; // max 2
//    int morphWidth;
//    int morphHeight;
    int iterations;
    MorphoParameters() {}
    MorphoParameters(int type, int iter):
        morphType(type), iterations(iter) {}
    MorphoParameters(const std::string& param)
    { fromString(param); }
    std::string typeToString() const;
    int typeFromString(const std::string&) const;
//    std::string shapeToString() const;
    std::string toString() const;
    void fromString(const std::string&);
    tinyxml2::XMLElement *save(tinyxml2::XMLDocument &doc) const;
    void saveAttribute(tinyxml2::XMLElement *node) const;
    void loadAttribute(const tinyxml2::XMLElement *node);
};

struct MorphoElement
{
    cv::Mat element;
    int morphShape;
    static MorphoElement getRandom();
    MorphoElement() {}
    MorphoElement(const std::string& elem, int nRows, int nCols)
    { fromString(elem, nRows, nCols); }
    std::string shapeToString() const;
    int shapeFromString(const std::string& shape);
    std::string toString() const;
    void fromString(const std::string&, int nRows, int nCols);
    tinyxml2::XMLElement *save(tinyxml2::XMLDocument &doc) const;
    void saveAttribute(tinyxml2::XMLElement *node) const;
    void loadAttribute(const tinyxml2::XMLElement *node);
};

class MorphoNode : public Node
{
public:
    typedef int(*MorphoPtr) (const std::vector<cv::Mat>&, cv::Mat&,
                             const MorphoParameters&,
                             const cv::Mat& element);
    static int morphologyOperation(const std::vector<cv::Mat>&, cv::Mat&,
                                   const MorphoParameters&,
                                   const cv::Mat& element);

    static NodePtr create(unsigned int geneNumber);
    static NodePtr createFromXml(const tinyxml2::XMLElement *node);

public:
    void execute(const std::vector<cv::Mat>& src, cv::Mat& dst) const;
    void mutate();

private:
    MorphoNode(const NodeId& id, MorphoPtr, const MorphoParameters&,
                const MorphoElement& element);
    MorphoNode(const tinyxml2::XMLElement *node, MorphoPtr,
               const MorphoParameters& param, const MorphoElement& element);
    MorphoNode(const MorphoNode& rhs);

private:
    virtual void writeNode(std::string& nodeString) const;
    virtual NodePtr cloneNode() const;
    virtual void save(tinyxml2::XMLDocument& doc,
                      tinyxml2::XMLElement *node) const;
    virtual void save(tinyxml2::XMLElement *node) const;

private:
    MorphoPtr morphoOperation_;
    MorphoParameters parameters_;
    MorphoElement structElement_;
};

#endif // MORPHONODE_H
