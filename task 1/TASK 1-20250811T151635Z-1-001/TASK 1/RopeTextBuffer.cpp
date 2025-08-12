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

Rope::Node *Rope::rotateLeft(Rope::Node *x) {
    
}
// ==================== PUBLIC INTERFACE ====================

int Rope::length() const
{
    return getTotalLength(root);
}

// Todo