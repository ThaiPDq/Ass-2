#include "RopeTextBuffer.h"

// ----------------- DoublyLinkedList -----------------

template <typename T>
void DoublyLinkedList<T>::insertAtHead(T data) {
    Node* newNode = new Node(data, nullptr, head);
    if (head != nullptr) {
        head->prev = newNode;
    } else {
        tail = newNode;
    }
    head = newNode;
    count++;
}

template <typename T>
void DoublyLinkedList<T>::insertAtTail(T data) {
    Node* newNode = new Node(data, tail, nullptr);
    if (tail != nullptr) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
    count++;
}

template <typename T>
void DoublyLinkedList<T>::insertAt(int index, T data) {
    if (index < 0 || index > count) {
        throw std::out_of_range("Index is invalid!");
    }
    if (index == 0) {
        insertAtHead(data);
        return;
    }
    if (index == count) {
        insertAtTail(data);
        return;
    }
    // traverse to node at index
    Node* current = head;
    for (int i = 0; i < index; ++i) current = current->next;
    Node* newNode = new Node(data, current->prev, current);
    if (current->prev) current->prev->next = newNode;
    current->prev = newNode;
    count++;
}

template <typename T>
void DoublyLinkedList<T>::deleteAt(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index is invalid!");
    }
    Node* toDelete = head;
    for (int i = 0; i < index; ++i) toDelete = toDelete->next;
    if (toDelete->prev) toDelete->prev->next = toDelete->next;
    else head = toDelete->next;
    if (toDelete->next) toDelete->next->prev = toDelete->prev;
    else tail = toDelete->prev;
    delete toDelete;
    count--;
}

// non-const get (mutable)
template <typename T>
T &DoublyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    Node* current = head;
    for (int i = 0; i < index; ++i) current = current->next;
    return current->data;
}

// const-get
template <typename T>
T &DoublyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    Node* current = head;
    for (int i = 0; i < index; ++i) current = current->next;
    return current->data;
}

template <typename T>
int DoublyLinkedList<T>::indexOf(T item) const {
    Node* current = head;
    int index = 0;
    while (current) {
        if (current->data == item) return index;
        current = current->next; index++;
    }
    return -1;
}

template <typename T>
bool DoublyLinkedList<T>::contains(T item) const {
    return indexOf(item) != -1;
}

template <typename T>
int DoublyLinkedList<T>::size() const { return count; }

template <typename T>
void DoublyLinkedList<T>::reverse() {
    Node* current = head;
    Node* tmp = nullptr;
    while (current) {
        tmp = current->prev;
        current->prev = current->next;
        current->next = tmp;
        current = current->prev; // move to original next
    }
    tmp = head; head = tail; tail = tmp;
}

// convert2str now takes const T& and can be nullptr
template <typename T>
std::string DoublyLinkedList<T>::toString(std::string (*convert2str)(T &) = nullptr) const {
    string result = "[";
    Node* current = head;
    bool first = true;
    while (current) {
        if (!first) result += ", ";
        if (convert2str) result += convert2str(current->data);
        else {
            std::ostringstream oss; oss << current->data; result += oss.str();
        }
        first = false;
        current = current->next;
    }
    result += "]";
    return result;
}


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

    
}

// TODO: implement other methods of DoublyLinkedList

// ----------------- RopeTextBuffer -----------------
RopeTextBuffer::RopeTextBuffer()
    : cursorPos(0)
{
    // TODO TASK 2
}

RopeTextBuffer::~RopeTextBuffer()
{
    // TODO TASK 2
}

void RopeTextBuffer::insert(const string &s)
{
    // TODO TASK 2
}

void RopeTextBuffer::deleteRange(int length)
{
    // TODO TASK 2
}

void RopeTextBuffer::replace(int length, const string &s)
{
    // TODO TASK 2
}

void RopeTextBuffer::moveCursorTo(int index)
{
    // TODO TASK 2
}

void RopeTextBuffer::moveCursorLeft()
{
    // TODO TASK 2
}

void RopeTextBuffer::moveCursorRight()
{
    // TODO TASK 2
}

int RopeTextBuffer::getCursorPos() const
{
    // TODO TASK 2
}

string RopeTextBuffer::getContent() const
{
    // TODO TASK 2
}

int RopeTextBuffer::findFirst(char c) const
{
    // TODO TASK 2
}

int *RopeTextBuffer::findAll(char c) const
{
    // TODO TASK 2
}

void RopeTextBuffer::undo()
{
    if (!history->canUndo())
        return;
    history->undo();
}

void RopeTextBuffer::redo()
{
    if (!history->canRedo())
        return;
    history->redo();
}

void RopeTextBuffer::printHistory() const
{
    history->printHistory();
}

void RopeTextBuffer::clear()
{
    // TODO TASK 2
}

// TODO: implement other methods of TextBuffer

// ----------------- HistoryManager -----------------
HistoryManager::HistoryManager() : currentIndex(0) {}

HistoryManager::~HistoryManager() {}

void HistoryManager::addAction(const Action &a)
{
    // TODO TASK 1
}

bool HistoryManager::canUndo() const
{
    // TODO TASK 2
}

bool HistoryManager::canRedo() const
{
    // TODO TASK 2
}

void HistoryManager::undo()
{
    if (canUndo())
        --currentIndex;
}

void HistoryManager::redo()
{
    if (canRedo())
        ++currentIndex;
}

void HistoryManager::printHistory() const
{
    // TODO TASK 2
}

//TODO: implement other methods of HistoryManager
