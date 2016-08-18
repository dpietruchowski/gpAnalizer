#ifndef TREE_H
#define TREE_H

#include <vector>
#include <cstdio>
#include <iostream>

#include "node/node.h"
#include "node/funcnode.h"
#include "node/termnode.h"
#include "node/morphonode.h"
#include "node/nullnode.h"
#include "functionset.h"
#include "terminalset.h"

class tree;
typedef std::unique_ptr<tree> tree_ptr;

enum typeInit
{
    GROW_INIT,
    FULL_INIT
};

class tree
{
private:
    int depth;
    int counterNodes;
     /*!
      * pxty
      * x - numer populacji
      * y - numer drzewa
      */
    std::string id;
private:
    node_ptr root;
public:
    tree(int depth, std::string id);
    tree(int depth);
    ~tree();
public:
        /*!
         * \brief initialize Initialize tree with random nodes
         * Leaf are always terminals, which represtented termNode
         * \param type If FULL_INIT then all leaves are at the same depth.
         * type If GROW_INIT then some of leaves can be at lower depth than tree depth
         * \param functions Contains function set
         * \param terminals Contains terminal set
         */
    void initialize(typeInit type, const functionSet &functions,
                    const terminalSet &terminals);
        /*!
         * \brief show Show tree structures on cout.
         */
    void show();
        /*!
         * \brief run Execute all nodes starting at terminals.
         * \return The result of the algorith represented by tree.
         */
    Mat run();
        /*!
         * \brief copy Copy this tree with new id.
         * \param New id.
         * \return New tree, which is copy of this tree.
         */
    tree_ptr copy(std::string newId);
        /*!
         * \brief copySubtree Copy subtree, where root is node of this tree with number subrootI
         * Set also id new tree.
         * \param subrootI Number of new root.
         * \param newId
         * \return New tree, which is copy of this subtree.
         */
    tree_ptr copySubtree(int subrootI, std::string newId);
        /*!
         * \brief copy Copy this tree.
         * \return New tree, which is copy of this tree.
         */
    tree_ptr copy();
        /*!
         * \brief copySubtree Copy subtree, where root is node of this tree with number subrootI
         * \param subrootI Number of new root.
         * \return New tree, which is copy of this subtree.
         */
    tree_ptr copySubtree(int subrootI);
public:
        /*!
         * \brief setId Set id this tree id.
         * Set also id of every node in this tree.
         * \param id New id.
         */
    void setId(std::string id);
        /*!
         * \brief setRoot Set root without children of new root
         * Children of this root will become children of new root.
         * If new root function has different
         * number of arguments then throw exception.
         * \param root New root.
         */
    void setRoot(node_ptr root);
        /*!
         * \brief setNode Set subtree with new node as root of new subtree.
         * If i is higher than tree size throw excetpion
         * \param i Node of this number will be change with root of new subtree.
         * \param newNode Root of new subtree.
         */
    void setSubtree(int i, node_ptr newNode);
        /*!
         * \brief returnRoot This root is no longer part of tree.
         * This tree will become empty.
         * \return This root.
         */
    node_ptr returnRoot();
        /*!
         * \brief writeTree Write this tree into the string.
         * \return
         */
    std::string writeTree();
    int getSize() const;
    int getDepth() const;
    int getSubtreeDepth(int subrootI) const;
    node* getRoot() const;
    node* getNode(int i) const;
    std::string getId() const;
private:
    void initializeRoot(const functionSet& functions, const terminalSet& terminals);
    int initializeGrow(const functionSet& functions, const terminalSet& terminals,
                       node* subroot, int depth);
    int initializeFull(const functionSet& functions, const terminalSet& terminals,
                       node* subroot, int depth);
    Mat runSubtree(node* subroot);
    int copySubtree(node* subroot, node* newSubroot);
    void showSubtree(node* subroot, std::string cou);
    void setSubtreeId(node* subroot);
    void checkDepth(node* subroot, int depth);
    void setSubtree(node* subroot, int i, node_ptr newNode);
    int getSubtreeSize(node* subroot) const;
    node* getNode(node* subroot, int n) const;
    std::string writeSubtree(node* subroot, std::string arrows);
private:
    node_ptr getRandomNode(const functionSet &functions);
    int getSubtreeDepth(node* subroot, int depth) const;

};

#endif // TREE_H
