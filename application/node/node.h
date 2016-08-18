#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <stdio.h>
#include "application/typeFunc.h"
#include "application/functionset.h"

class node;
typedef std::unique_ptr<node> node_ptr;

enum typeNode
{
    TERMINAL_NODE,
    FUNCTION_NODE,
    MORPHOLOGY_NODE
};

class node
{
private:
    std::vector<node_ptr> children;
    /*!
     * x - numer populacji
     * y - numer drzewa
     * z - numer węzła
     */
    std::string id;
    typeNode type;

public:
    node(std::string id, typeNode type);
    virtual ~node() {}
    void addChild(node_ptr child);
    void setChild(int i, node_ptr newChild);
    void setId(std::string id);
    node_ptr returnChild(int i);
    std::string writeNode();

    node* getChild(int i) const;
    int getChildrenSize() const;
    std::string getId() const;
    typeNode getType() const;

    void show();

    virtual std::string getInfo() = 0;
    virtual node_ptr copyAttribute() = 0;
    virtual void execute(const std::vector<Mat>& src, Mat& dst) = 0;
    virtual int getArgumentsNumber() = 0;
};

#endif // NODE_H
