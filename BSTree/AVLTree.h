#pragma once

#include <vector>

class AVLTree
{
public:

    AVLTree();
    ~AVLTree();
    void Insert(int key);
    void Remove(int key);
    bool Find(int key);
    int Height();
    void Clear();
    void Print();
    std::vector<int> GetVector();

private:

    struct Node
    {
        int key;
        Node* left;
        Node* right;
        unsigned char height;

        Node(int key);
        ~Node();
    };

    unsigned char Height(Node* node);
    void FixHeight(Node* node);
    int BalanceFactor(Node* node);
    Node* RotateLeft(Node* p);
    Node* RotateRight(Node* p);
    Node* RotateRightLeft(Node* p);
    Node* RotateLeftRight(Node* p);
    Node* Balance(Node* node);
    Node* Insert(Node* node, int key);
    Node* FindMin(Node* node);
    Node* ExcludeMin(Node* node);
    Node* Remove(Node* node, int key);
    void Print(Node* node);
    void GetVector(Node* node, std::vector<int>& vec);

    Node* root;
};
