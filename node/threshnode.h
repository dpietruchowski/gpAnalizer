#ifndef THRESHNODE_H
#define THRESHNODE_H

#include "node.h"

struct ThreshParameters
{
    static ThreshParameters getRandom();
    int threshValue;
    int threshType; //max 4

    ThreshParameters() {}
    ThreshParameters(int value, int type):
        threshValue(value), threshType(type) {}
    ThreshParameters(const std::string& param)
    { fromString(param); }
    std::string typeToString() const;
    int typeFromString(const std::string&) const;
    std::string toString() const;
    void fromString(const std::string&);
    tinyxml2::XMLElement *save(tinyxml2::XMLDocument &doc) const;
    void saveAttribute(tinyxml2::XMLElement *node) const;
    void loadAttribute(const tinyxml2::XMLElement *node);
};

class ThreshNode : public Node
{
public:
    typedef int(*ThreshPtr) (const std::vector<cv::Mat>&, cv::Mat&,
                             const ThreshParameters&);
    static int thresholdOperation(const std::vector<cv::Mat>&, cv::Mat&,
                                   const ThreshParameters&);
    static NodePtr create(unsigned int geneNumber);
    static NodePtr createFromXml(const tinyxml2::XMLElement *node);

public:
    void execute(const std::vector<cv::Mat>& src, cv::Mat& dst) const;
    void mutate();

private:
    ThreshNode(const NodeId& id, ThreshPtr threshOperation, const ThreshParameters&param);
    ThreshNode(const tinyxml2::XMLElement *node, ThreshPtr,
               const ThreshParameters& param);
    ThreshNode(const ThreshNode& rhs);

private:
    virtual void writeNode(std::string& nodeString) const;
    virtual NodePtr cloneNode() const;
    virtual void save(tinyxml2::XMLDocument& doc,
                      tinyxml2::XMLElement *node) const;
    virtual void save(tinyxml2::XMLElement *node) const;

private:
    ThreshPtr threshOperation_;
    ThreshParameters parameters_;
};

#endif // THRESHNODE_H
