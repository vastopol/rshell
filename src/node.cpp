// Node class implementation

#include "node.h"


Node::Node(std::string s)
    : key(s), parent(0), left(0), right(0)
{}
//-----------------------------------------------   

Node::~Node()
{
}
//------------------------------------------------

void Node::setPar(Node* n)
{
}
//-----------------------------------------------

void Node::setLeft(Node* n)
{
}
//-----------------------------------------------

void Node::setRight(Node* n)
{
}
//-----------------------------------------------

Node* Node::getPar()
{
    return parent;
}
//-----------------------------------------------

Node* Node::getLeft()
{
    return left;
}
//-----------------------------------------------

Node* Node::getRight()
{
    return right;
}
//-----------------------------------------------

std::string Node::getKey()
{
    return key;
}
//-----------------------------------------------

