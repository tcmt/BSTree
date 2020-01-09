#include "RBTree.h"
#include <cassert>
#include <algorithm>

RBTree::Node::Node() :
    left{ nullptr },
    right{ nullptr },
    parent{ nullptr },
    color{ Color::Red },
    key{ 0 }
{
}

RBTree::Node::Node(int key) :
    left{ nullptr },
    right{ nullptr },
    parent{ nullptr },
    color{ Color::Red },
    key{ key }
{
}

RBTree::RBTree()
{
    nil->key = 0;
    nil->color = Color::Black;
    nil->left = nullptr;
    nil->right = nullptr;
    nil->parent = nullptr;
}

RBTree::~RBTree()
{
    DeleteNodesRecursively(root);
}

RBTree::Node* RBTree::NewNode(int key)
{
    Node* node = new Node;
    node->key = key;
    node->color = Color::Red;
    node->left = nil;
    node->right = nil;
    node->parent = nil;
    return node;
}

void RBTree::DeleteNode(Node* node)
{
    delete node;
}

void RBTree::DeleteNodesRecursively(Node* node)
{
    if (node == nil)
    {
        return;
    }
    DeleteNodesRecursively(node->left);
    DeleteNodesRecursively(node->right);
    DeleteNode(node);
}

void RBTree::RotateLeft(Node* p)
{
    assert(p != nil);
    assert(p->right != nil);
    assert(root->parent == nil);

    Node* q = p->right;

    // p - c link
    p->right = q->left;
    if (p->right != nil)
    {
        p->right->parent = p;
    }
    // q - parent link
    q->parent = p->parent;
    if (q->parent == nil)
    {
        root = q;
        assert(root->parent == nil);
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
}

void RBTree::RotateRight(Node* p)
{
    assert(p != nil);
    assert(p->left != nil);
    assert(root->parent == nil);

    Node* q = p->left;

    // p - c link
    p->left = q->right;
    if (p->left != nil)
    {
        p->left->parent = p;
    }
    // q - parent link
    q->parent = p->parent;
    if (q->parent == nil)
    {
        root = q;
        assert(root->parent == nil);
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
}

void RBTree::Insert(int key)
{
    Node* node = InsertNode(key);
    if (node == nullptr)
    {
        return;
    }
    InsertFixup(node);
}

RBTree::Node* RBTree::InsertNode(int key)
{
    Node* parent = nil;
    Node* node = root;

    // find insertion position
    while (node != nil)
    {
        if (key == node->key)
        {
            return nullptr;
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

    // insert
    node = NewNode(key);
    node->parent = parent;
    if (parent == nil)
    {
        root = node;
        assert(root->parent == nil);
    }
    else
    {
        if (key < parent->key)
        {
            parent->left = node;
        }
        else
        {
            parent->right = node;
        }
    }

    return node;
}

void RBTree::InsertFixup(Node* node)
{
    while (node->parent->color == Color::Red)
    {
        Node* parent = node->parent;
        Node* grandparent = node->parent->parent;
        if (parent == grandparent->left)
        {
            // parent is left child

            Node* uncle = grandparent->right;
            if (uncle->color == Color::Red)
            {
                // Case 1. Red parent, Red uncle
                parent->color = Color::Black;
                uncle->color = Color::Black;
                grandparent->color = Color::Red;
                node = grandparent;
            }
            else
            {
                // Case 2. Red parent, Black uncle, node is right child
                if (node == parent->right)
                {
                    node = node->parent;
                    RotateLeft(node);
                    parent = node->parent;
                    grandparent = node->parent->parent;
                    // node is left child now
                }

                // Case 3. Red parent, Black uncle, node is left child
                parent->color = Color::Black;
                grandparent->color = Color::Red;
                RotateRight(grandparent);
            }
        }
        else
        {
            // parent is right child

            Node* uncle = grandparent->left;
            if (uncle->color == Color::Red)
            {
                // Case 1. Red parent, Red uncle
                parent->color = Color::Black;
                uncle->color = Color::Black;
                grandparent->color = Color::Red;
                node = grandparent;
            }
            else
            {
                // Case 2. Red parent, Black uncle, node is left child
                if (node == parent->left)
                {
                    node = node->parent;
                    RotateRight(node);
                    parent = node->parent;
                    grandparent = node->parent->parent;
                    // node is right child now
                }

                // Case 3. Red parent, Black uncle, node is right child
                parent->color = Color::Black;
                grandparent->color = Color::Red;
                RotateLeft(grandparent);
            }
        }
    }
    // Case 4. Fix root
    root->color = Color::Black;
    assert(root->parent == nil);
}

RBTree::Node* RBTree::FindNode(int key)
{
    assert(root->parent == nil);

    Node* node = root;
    while (node != nil)
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
    return nil;
}

RBTree::Node* RBTree::FindMin(Node* node)
{
    while (node->left != nil)
    {
        node = node->left;
    }
    return node;
}

void RBTree::Remove(int key)
{
    RemoveNode(key);
}

void RBTree::RemoveNode(int key)
{
    Node* node = FindNode(key);
    if (node == nil)
    {
        return;
    }
    // find removing/replacing node y and its child x
    Node* y = node;
    Node* x = nil;
    if (node->left == nil)
    {
        x = node->right;
    }
    else if (node->right == nil)
    {
        x = node->left;
    }
    else
    {
        y = FindMin(node->right);
        x = y->right;
    }
    // remove/replace
    x->parent = y->parent;
    if (x->parent == nil)
    {
        root = x;
        assert(root->parent == nil);
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
    // fixup
    if (y->color == Color::Black)
    {
        RemoveFixup(x);
    }

    DeleteNode(y);
}

void RBTree::RemoveFixup(Node* node)
{
    assert(root->parent == nil);

    while (node != root && node->color == Color::Black)
    {
        if (node == node->parent->left)
        {
            Node *s = node->parent->right;
            // Case 1. node's sibling s - Red
            if (s->color == Color::Red)
            {
                s->color = Color::Black;
                node->parent->color = Color::Red;
                RotateLeft(node->parent);
                s = node->parent->right;
            }
            // Case 2. node's sibling s - Black, s_left - Black, s_right - Black
            if (s->left->color == Color::Black && s->right->color == Color::Black)
            {
                s->color = Color::Red;
                node = node->parent;
            }
            else
            {
                // Case 3. node's sibling s - Black, s_left - Red, s_right - Black
                if (s->right->color == Color::Black)
                {
                    s->left->color = Color::Black;
                    s->color = Color::Red;
                    RotateRight(s);
                    s = node->parent->right;
                }
                // Case 4. node's sibling s - Black, s_right - Red
                s->color = node->parent->color;
                node->parent->color = Color::Black;
                s->right->color = Color::Black;
                RotateLeft(node->parent);
                node = root;
                assert(root->parent == nil);
            }
        }
        else
        {
            Node *w = node->parent->left;
            // Case 1. node's sibling s - Red
            if (w->color == Color::Red)
            {
                w->color = Color::Black;
                node->parent->color = Color::Red;
                RotateRight(node->parent);
                w = node->parent->left;
            }
            // Case 2. node's sibling s - Black, s_left - Black, s_right - Black
            if (w->left->color == Color::Black && w->right->color == Color::Black)
            {
                w->color = Color::Red;
                node = node->parent;
            }
            else
            {
                // Case 3. node's sibling s - Black, s_left - Black, s_right - Red
                if (w->left->color == Color::Black)
                {
                    w->right->color = Color::Black;
                    w->color = Color::Red;
                    RotateLeft(w);
                    w = node->parent->left;
                }
                // Case 4. node's sibling s - Black, s_left - Red
                w->color = node->parent->color;
                node->parent->color = Color::Black;
                w->left->color = Color::Black;
                RotateRight(node->parent);
                node = root;
                assert(root->parent == nil);
            }
        }
    }
    // Case 5. Fix root
    node->color = Color::Black;
    assert(root->parent == nil);
}

bool RBTree::Find(int key)
{
    return FindNode(key) != nil;
}

void RBTree::Clear()
{
    DeleteNodesRecursively(root);
    root = nil;
}

void RBTree::GetVector(Node* node, std::vector<int>& vec)
{
    if (node == nil)
    {
        return;
    }
    GetVector(node->left, vec);
    vec.push_back(node->key);
    GetVector(node->right, vec);
}

std::vector<int> RBTree::GetVector()
{
    std::vector<int> values;
    GetVector(root, values);
    return values;
}

size_t RBTree::Height()
{
	return Height(root);
}

size_t RBTree::Height(Node* node)
{
	if (node == nullptr)
	{
		return 0;
	}

	return std::max(Height(node->left), Height(node->right)) + 1;
}