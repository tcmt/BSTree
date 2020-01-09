#include "AVLTree.h"
#include <algorithm>
#include <iostream>

AVLTree::Node::Node(int key) :
    key{ key },
    left{ nullptr },
    right{ nullptr },
    height{ 1 }
{
}

AVLTree::Node::~Node()
{
    delete left;
    delete right;
}

AVLTree::AVLTree() :
    root{ nullptr }
{
}

AVLTree::~AVLTree()
{
    delete root;
}

int AVLTree::Height()
{
    return Height(root);
}

void AVLTree::Insert(int key)
{
    root = Insert(root, key);
}

void AVLTree::Remove(int key)
{
    root = Remove(root, key);
}

bool AVLTree::Find(int key)
{
    Node* node = root;
    while (node != nullptr)
    {
        if (node->key == key)
        {
            return true;
        }
        if (key < node->key)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    return false;
}

void AVLTree::Clear()
{
    delete root;
    root = nullptr;
}

void AVLTree::Print()
{
    Print(root);
}

std::vector<int> AVLTree::GetVector()
{
    std::vector<int> values;
    GetVector(root, values);
    return values;
}

unsigned char AVLTree::Height(Node* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

void AVLTree::FixHeight(Node* node)
{
    node->height = std::max(Height(node->left), Height(node->right)) + 1;
}

int AVLTree::BalanceFactor(Node* node)
{
    return Height(node->left) - Height(node->right);
}

AVLTree::AVLTree::Node* AVLTree::RotateLeft(Node* p)
{
    Node* q = p->right;
    p->right = q->left;
    q->left = p;
    FixHeight(p);
    FixHeight(q);
    return q;
}

AVLTree::AVLTree::Node* AVLTree::RotateRight(Node* p)
{
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    FixHeight(p);
    FixHeight(q);
    return q;
}

AVLTree::AVLTree::Node* AVLTree::RotateRightLeft(Node* p)
{
    p->right = RotateRight(p->right);
    return RotateLeft(p);
}

AVLTree::AVLTree::Node* AVLTree::RotateLeftRight(Node* p)
{
    p->left = RotateLeft(p->left);
    return RotateRight(p);
}

AVLTree::AVLTree::Node* AVLTree::Balance(Node* node)
{
    FixHeight(node);

    int balance = BalanceFactor(node);
    if (balance == 2)
    {
        if (BalanceFactor(node->left) >= 0)
        {
            return RotateRight(node);
        }
        else
        {
            return RotateLeftRight(node);
        }
    }
    if (balance == -2)
    {
        if (BalanceFactor(node->right) <= 0)
        {
            return RotateLeft(node);
        }
        else
        {
            return RotateRightLeft(node);
        }
    }

    return node;
}

AVLTree::AVLTree::Node* AVLTree::Insert(Node* node, int key)
{
    if (node == nullptr)
    {
        node = new Node(key);
        return node;
    }

    if (key == node->key)
    {
        return node;
    }

    if (key < node->key)
    {
        node->left = Insert(node->left, key);
    }
    else
    {
        node->right = Insert(node->right, key);
    }

    return Balance(node);
}

AVLTree::AVLTree::Node* AVLTree::FindMin(Node* node)
{
    if (node->left == nullptr)
    {
        return node;
    }
    return FindMin(node->left);
}

AVLTree::AVLTree::Node* AVLTree::ExcludeMin(Node* node)
{
    if (node->left == nullptr)
    {
        return node->right;
    }
    node->left = ExcludeMin(node->left);
    return Balance(node);
}

AVLTree::AVLTree::Node* AVLTree::Remove(Node* node, int key)
{
    if (node == nullptr)
    {
        return node;
    }

    if (key < node->key)
    {
        node->left = Remove(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = Remove(node->right, key);
    }
    else
    {
        Node* left = node->left;
        Node* right = node->right;
        node->left = nullptr;
        node->right = nullptr;
        delete node;
        if (left == nullptr)
        {
            return right;
        }
        if (right == nullptr)
        {
            return left;
        }
        Node* newNode = FindMin(right);
        newNode->right = ExcludeMin(right);
        newNode->left = left;
        return Balance(newNode);
    }

    return Balance(node);
}

void AVLTree::Print(Node* node)
{
    if (node == nullptr)
    {
        return;
    }
    Print(node->left);
    std::cout << node->key << " ";
    Print(node->right);
}

void AVLTree::GetVector(Node* node, std::vector<int>& vec)
{
    if (node == nullptr)
    {
        return;
    }
    GetVector(node->left, vec);
    vec.push_back(node->key);
    GetVector(node->right, vec);
}
