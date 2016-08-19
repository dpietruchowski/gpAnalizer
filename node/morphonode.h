#ifndef MORPHONODE_H
#define MORPHONODE_H

#include "node.h"

struct MorphoParameters
{
    const static int MAX_WIDTH = 15;
    const static int MAX_HEIGHT = 15;
    const static int MAX_ITER = 40;
    static MorphoParameters getRandom();
    int morphType; // max 7
//    int morphShape; // max 2
//    int morphWidth;
//    int morphHeight;
    int iterations;
    MorphoParameters(int type, int iter):
        morphType(type), iterations(iter) {}
    MorphoParameters(const std::string& param)
    { fromString(param); }
    std::string typeToString() const;
    int typeFromString(std::string&) const;
//    std::string shapeToString() const;
    std::string toString() const;
    void fromString(const std::string&);
};

struct MorphoElement
{
    cv::Mat element;
    static MorphoElement getRandom();
    MorphoElement() {}
    MorphoElement(const std::string& elem, int nRows, int nCols)
    { fromString(elem, nRows, nCols); }
    std::string toString() const;
    void fromString(const std::string&, int nRows, int nCols);
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

public:
    void execute(const std::vector<cv::Mat>& src, cv::Mat& dst) const;
    void mutate();

private:
    MorphoNode(const NodeId& id, MorphoPtr, const MorphoParameters&,
                const MorphoElement& element);
    MorphoNode(const MorphoNode& rhs);

private:
    virtual void writeNode(std::string& nodeString) const;
    virtual NodePtr cloneNode() const;

private:
    MorphoPtr morphoOperation_;
    MorphoParameters parameters_;
    MorphoElement structElement_;
};

#endif // MORPHONODE_H
