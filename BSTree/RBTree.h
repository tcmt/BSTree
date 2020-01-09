#pragma once

#include <vector>

class RBTree
{
public:
    RBTree();
    ~RBTree();

    void Insert(int key);
    void Remove(int key);
    bool Find(int key);
    void Clear();
    std::vector<int> GetVector();
	size_t Height();

private:

    enum class Color { Black, Red };

    struct Node
    {
        Node *left;
        Node *right;
        Node *parent;
        Color color;
        int key;

        Node();
        Node(int key);
    };

    Node* NewNode(int key);
    void DeleteNode(Node* node);
    void DeleteNodesRecursively(Node* node);

    void RotateLeft(Node* p);
    void RotateRight(Node* p);
    Node* InsertNode(int key);
    void InsertFixup(Node* node);
    Node* FindNode(int key);
    Node* FindMin(Node* node);
    void RemoveNode(int key);
    void RemoveFixup(Node* node);

    void GetVector(Node* node, std::vector<int>& vec);
	size_t Height(Node* node);

    Node sentinel;
    Node* const nil = &sentinel;
    Node *root = nil;
};

