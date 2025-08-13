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

//helper min
int min(int a, int b) {
    return (a < b) ? a : b;
}

Rope::Node *Rope::rotateLeft(Rope::Node *x) 
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    update(x); update(y);
    return y;
}

Rope::Node * Rope::rotateRight(Rope::Node *y) {
    Node *x = y->left;
    Node *t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    update(y); update(x);
    return x;
}

Rope::Node * Rope::rebalance(Rope::Node *node) {
    if (node == nullptr) return node;

    node->height = max(height(node->left), height(node->right)) + 1;

    int balance = height(node->left) - height(node->right);
    
    //left heavy
    if (balance > 1) {
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
    else if (balance < -1) {
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

char Rope::charAt(Rope::Node *node, int index) const {
    if (!node) return '\0';
    
    if (node->isLeaf()) {
        if (index <0 ||index > node->data.size()) return '\0';

        return node->data[index];
    }

    if (index < node->weight) {
        return charAt(node->left, index);
    }
    else {
        return charAt(node->right, index - node->weight);
    }
}

std::string Rope::toString(Rope::Node *node) const {
    if (!node) return "";

    if (node->isLeaf()) return node->data;

    string leftStr = toString(node->left);
    string rStr = toString(node->right);

    return leftStr + rStr;
}

void Rope::destroy(Rope::Node *&node) {
    if (!node) return;

    destroy(node->left);
    destroy(node->right);

    delete node;
    node = nullptr;
}

void Rope::split(Node* node, int index, Node*& outLeft, Node*& outRight) {
    if (!node) {
        outLeft = nullptr;
        outRight = nullptr;
        return;
    }

    if (!node->left && !node->right) {
        if (index <= 0) {
            outLeft = nullptr;
            outRight = new Node(node->data);
        }
        else if (index >= node->data.size()) {
            outLeft = new Node(node->data);
            outRight = nullptr;
        }
        else {
            std::string leftStr = node->data.substr(0, index);
            std::string rightStr = node->data.substr(index);
            outLeft = leftStr.empty() ? nullptr : new Node(leftStr);
            outRight = rightStr.empty() ? nullptr : new Node(rightStr);
        }
        return;
    }
    if (index < node->weight) {
        Node* leftPart = nullptr;
        Node* rightPart = nullptr;
        split(node->left, index, leftPart, rightPart);
        outLeft = leftPart;
        outRight = concatNodes(rightPart, node->right);
        update(outRight);
    }
    else if (index > node->weight) {
        Node* leftPart = nullptr;
        Node* rightPart = nullptr;
        split(node->right, index - node->weight, leftPart, rightPart);
        outLeft = concatNodes(node->left, leftPart);
        outRight = rightPart;
        update(outLeft);
    }
    else {
        outLeft = node->left;
        outRight = node->right;
    }
}

// ==================== PUBLIC INTERFACE ====================

int Rope::length() const
{
    return getTotalLength(root);
}

bool Rope::empty() const {
    return root == nullptr;
}

char Rope::charAt(int index) const {
    return this->charAt(root, index);
}

std::string Rope::substringHelper(Rope::Node *node, int start, int end) const {
    if (!node || start>end) return "";

    if (node->isLeaf()) {
        int leafLeng = node->data.size();
        int starto = max(0, start);
        int endo = min(leafLeng - 1, end);
        if (starto > endo) return "";
        return node->data.substr(starto, endo + 1 - starto);
    }

    if (end < node->weight) {
        return substringHelper (node->left, start, end);
    }
    else if (start >= node->weight) {
        return substringHelper (node->right, start - node->weight, end - node->weight);
    }
    else {
        string lefty = substringHelper(node->left, start, node->weight - 1);
        string righty = substringHelper(node->right, 0, end - node->weight);
        return lefty+righty;
    }
}

std::string Rope::substring(int start, int length) const {
    return substringHelper(root, start, start+length - 1);
}

void Rope::insert(int index, const std::string &s) {
    if (index < 0 || index > length()) {
        throw std::out_of_range("Index is invalid!");
    }

    if (s.empty()) return;

    if (!root) {
        int len = s.size();
        // First chunk becomes root
        root = new Node(s.substr(0, CHUNK_SIZE));

        // Remaining chunks concatenated
        int start = CHUNK_SIZE;
        while (start < s.size()) {
            int chunkSize = (len - start > CHUNK_SIZE) ? CHUNK_SIZE : (len - start);
            Node* newNode = new Node(s.substr(start, chunkSize));
            root = concatNodes(root, newNode); // concatNodes should handle balancing if needed
            start += chunkSize;
        }
        return;
    }

    Node* left = nullptr;
    Node* right = nullptr;

    split(root, index, left, right);

    Node* middle = new Node(s);

    Node* temp = concatNodes(left, middle);

    root = concatNodes(temp, right);
}

void Rope::deleteRange(int start, int length) {
    Node* s1 = nullptr;
    Node* s2 = nullptr;

    split(root, start, s1, s2);

    Node* s3 = nullptr;
    Node* s4 = nullptr;
    split(s2, length, s3, s4);

    root = concatNodes(s1, s4);
    destroy(s3);
}

std::string Rope::toString() const {
    string result = toString(root);
    return result;
}

// Todo