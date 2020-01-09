#include "AVLTreeIterative.h"
#include <algorithm>
#include <cassert>

AVLTreeIterative::Node::Node(int key) :
    key{ key },
    left{ nullptr },
    right{ nullptr },
    parent{ nullptr },
    height{ 1 }
{
}

AVLTreeIterative::Node::~Node()
{
    delete left;
    delete right;
}

AVLTreeIterative::AVLTreeIterative() :
    root { nullptr }
{
}

AVLTreeIterative::~AVLTreeIterative()
{
    delete root;
}

void AVLTreeIterative::Insert(int key)
{
    InsertNode(key);
}

void AVLTreeIterative::Remove(int key)
{
    RemoveNode(key);
}

bool AVLTreeIterative::Find(int key)
{
    return FindNode(key) != nullptr;
}

void AVLTreeIterative::Clear()
{
    delete root;
    root = nullptr;
}

std::vector<int> AVLTreeIterative::GetVector()
{
    std::vector<int> vec;
    GetVector(root, vec);
    return vec;
}

unsigned char AVLTreeIterative::Height(Node* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

void AVLTreeIterative::FixHeight(Node* node)
{
    node->height = std::max(Height(node->left), Height(node->right)) + 1;
}

int AVLTreeIterative::BalanceFactor(Node* node)
{
    return Height(node->left) - Height(node->right);
}

void AVLTreeIterative::RotateLeft(Node* p)
{
    assert(p != nullptr);
    assert(p->right != nullptr);
    assert(root->parent == nullptr);

    Node* q = p->right;

    // p - c link
    p->right = q->left;
    if (p->right != nullptr)
    {
        p->right->parent = p;
    }
    // q - parent link
    q->parent = p->parent;
    if (q->parent == nullptr)
    {
        root = q;
        assert(root->parent == nullptr);
    }
    else
    {
        if (p == p->parent->left)
        {
            q->parent->left = q;
        }
        else
        {
            q->parent->right = q;
        }
    }
    // p - q link
    q->left = p;
    p->parent = q;

    FixHeight(p);
    FixHeight(q);
}

void AVLTreeIterative::RotateRight(Node* p)
{
    assert(p != nullptr);
    assert(p->left != nullptr);
    assert(root->parent == nullptr);

    Node* q = p->left;

    // p - c link
    p->left = q->right;
    if (p->left != nullptr)
    {
        p->left->parent = p;
    }
    // q - parent link
    q->parent = p->parent;
    if (q->parent == nullptr)
    {
        root = q;
        assert(root->parent == nullptr);
    }
    else
    {
        if (p == p->parent->left)
        {
            q->parent->left = q;
        }
        else
        {
            q->parent->right = q;
        }
    }
    // p - q link
    q->right = p;
    p->parent = q;

    FixHeight(p);
    FixHeight(q);
}

void AVLTreeIterative::RotateRightLeft(Node* p)
{
    RotateRight(p->right);
    RotateLeft(p);
}

void AVLTreeIterative::RotateLeftRight(Node* p)
{
    RotateLeft(p->left);
    RotateRight(p);
}

void AVLTreeIterative::InsertBalance(Node* node)
{
    while (node != nullptr)
    {
        FixHeight(node);
        int balance = BalanceFactor(node);
        if (balance == 0)
        {
            break;
        }
        if (balance == 2)
        {
            if (BalanceFactor(node->left) > 0)
            {
                RotateRight(node);
            }
            else
            {
                RotateLeftRight(node);
            }
            break;
        }
        if (balance == -2)
        {
            if (BalanceFactor(node->right) < 0)
            {
                RotateLeft(node);
            }
            else
            {
                RotateRightLeft(node);
            }
            break;
        }
        node = node->parent;
    }
}

void AVLTreeIterative::RemoveBalance(Node* node)
{
    while (node != nullptr)
    {
        int balance = BalanceFactor(node);
        if (balance == 0)
        {
            break;
        }
        FixHeight(node);
        if (balance == 2)
        {
            if (BalanceFactor(node->left) > 0)
            {
                RotateRight(node);
                node = node->parent;
            }
            else if (BalanceFactor(node->left) == 0)
            {
                RotateRight(node);
                break;
            }
            else
            {
                RotateLeftRight(node);
                node = node->parent;
            }
        }
        if (balance == -2)
        {
            if (BalanceFactor(node->right) < 0)
            {
                RotateLeft(node);
                node = node->parent;
            }
            else if (BalanceFactor(node->right) == 0)
            {
                RotateLeft(node);
                break;
            }
            else
            {
                RotateRightLeft(node);
                node = node->parent;
            }
        }
        node = node->parent;
    }
}

AVLTreeIterative::Node* AVLTreeIterative::FindNode(int key)
{
    Node* node = root;
    while (node != nullptr)
    {
        if (node->key == key)
        {
            return node;
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
    return nullptr;
}

AVLTreeIterative::Node* AVLTreeIterative::FindMin(Node* node)
{
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

void AVLTreeIterative::InsertNode(int key)
{
    if (root == nullptr)
    {
        root = new Node(key);
        return;
    }

    Node* parent = nullptr;
    Node* node = root;
    // go down and find insertion position
    while (node != nullptr)
    {
        if (key == node->key)
        {
            return;
        }

        parent = node;
        if (key < node->key)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }

    // insert new node
    node = new Node(key);
    node->parent = parent;
    if (key < parent->key)
    {
        parent->left = node;
    }
    else
    {
        parent->right = node;
    }

    // go up and balance tree
    InsertBalance(parent);
}

void AVLTreeIterative::RemoveNode(int key)
{
    // find removing node
    Node* node = FindNode(key);
    if (node == nullptr)
    {
        return;
    }

    Node* y = node;
    Node* x = nullptr;
    // find y and its child x nodes
    if (node->left == nullptr)
    {
        x = node->right;
    }
    else if (node->right == nullptr)
    {
        x = node->left;
    }
    else
    {
        y = FindMin(node->right);
        x = y->right;
    }
    // exclude y
    if (x != nullptr)
    {
        x->parent = y->parent;
    }
    if (y->parent == nullptr)
    {
        root = x;
    }
    else
    {
        if (y == y->parent->left)
        {
            y->parent->left = x;
        }
        else
        {
            y->parent->right = x;
        }
    }
    if (y != node)
    {
        node->key = y->key;
    }
    
    // go up and balance tree
    RemoveBalance(y->parent);

    // delete y
    y->left = nullptr;
    y->right = nullptr;
    delete y;
}

void AVLTreeIterative::GetVector(Node* node, std::vector<int>& vec)
{
    if (node == nullptr)
    {
        return;
    }
    GetVector(node->left, vec);
    vec.push_back(node->key);
    GetVector(node->right, vec);
}

size_t AVLTreeIterative::Height()
{
	return Height(root);
}