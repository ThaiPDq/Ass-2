#include "RopeTextBuffer.h"

#ifdef TESTING
    friend class TestHelper;
#endif

// ==================== NODE IMPLEMENTATION ====================

Rope::Node::Node() : left(nullptr), right(nullptr), data(""), weight(0), height(1), balance(EH) {}

Rope::Node::Node(const string &s)
{
    left = nullptr;
    right = nullptr;
    data = s;
    weight = s.size();
    height = 1;
    balance = EH;
}

bool Rope::Node::isLeaf() const {
    return left == nullptr && right == nullptr;
}

// ==================== ROPE IMPLEMENTATION ====================

Rope::Rope() : root(nullptr) {}

Rope::~Rope()
{
    destroy(root);
}

// Todo

int Rope::height(Rope::Node *node) const {
    if (node == nullptr) return 0;
    return node->height;
}

// tính tổng các lá
int Rope::getTotalLength(Rope::Node* node) const {
    if (node == nullptr) return 0;
    
    if (node->isLeaf()) return node->data.size();

    return node->weight + getTotalLength(node->right);

}

void Rope::update(Rope::Node *node) {
    if (node == nullptr) return ;

    node->weight = getTotalLength (node->left);

    int h_left = height (node->left);
    int h_right = height (node->right);

    node->height = (h_left > h_right ? h_left : h_right) + 1;

    int height_diff = h_left - h_right;
    
    if (height_diff > 0) node->balance = Node::LH;
    else if (height_diff == 0) node->balance = Node::EH;
    else node->balance = Node::RH; 
}

//helper max
int max(int a, int b) {
    return (a > b) ? a : b;
}

Rope::Node *Rope::rotateLeft(Rope::Node *x) 
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Rope::Node * Rope::rotateRight(Rope::Node *y) {
    Node *x = y->left;
    Node *t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Rope::Node * Rope::rebalance(Rope::Node *node) {
    if (node == nullptr) return node;

    node->height = max(height(node->left), height(node->right));
    
    //left heavy
    if (node->balance > 1) {
        if (height(node->left->left) >= height(node->left->right)) {
            return rotateRight(node); //ll
        }
        else {
            //lr
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }
    //right
    else if (node->balance < -1) {
        if (height(node->right->right) >= height(node->right->left)) {
            return rotateLeft(node);
        }
        else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }
    return node;
}

Rope::Node *Rope::concatNodes(Rope::Node *left, Rope::Node *right) {
    Node* node = new Node("");

    node->left = left;
    node->right = right;

    node->weight = getTotalLength(left);
    node->height = max(height(left), height(right)) + 1 ;

    return rebalance(node);
}

// ==================== PUBLIC INTERFACE ====================

int Rope::length() const
{
    return getTotalLength(root);
}

// Todo