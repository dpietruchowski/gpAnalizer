#ifndef TERMNODE_H
#define TERMNODE_H

#include "node.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class termNode: public node
{
private:
    Mat_ptr terminal;
    std::string terminalName;
public:
    termNode(Mat_ptr image, std::string id);
    termNode(Mat_ptr image);

    std::string getInfo();
    node_ptr copyAttribute();
    void showNode();
    void execute(const std::vector<cv::Mat> &src, cv::Mat &dst);
    int getArgumentsNumber();
};

#endif // TERMNODE_H
