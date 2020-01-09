#pragma once

#include <vector>

class AVLTreeIterative
{
public:
	AVLTreeIterative();
	~AVLTreeIterative();

    void Insert(int key);
    void Remove(int key);
    bool Find(int key);
    void Clear();
    std::vector<int> GetVector();
	size_t Height();

private:

    struct Node
    {
        int key;
        Node* left;
        Node* right;
        Node* parent;
        unsigned char height;

        Node(int key);
        ~Node();
    };

    unsigned char Height(Node* node);
    void FixHeight(Node* node);
    int BalanceFactor(Node* node);

    void RotateLeft(Node* p);
    void RotateRight(Node* p);
    void RotateRightLeft(Node* p);
    void RotateLeftRight(Node* p);
    void InsertBalance(Node* node);
    void RemoveBalance(Node* node);
    Node* FindNode(int key);
    Node* FindMin(Node* node);
    void InsertNode(int key);
    void RemoveNode(int key);
    void GetVector(Node* node, std::vector<int>& vec);

    Node* root;
};

