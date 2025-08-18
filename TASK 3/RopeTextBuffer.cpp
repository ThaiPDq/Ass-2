#include "RopeTextBuffer.h"

#ifdef TESTING
    friend class TestHelper;
#endif
// ==================== DoublyLinkedList IMPLEMENTATION ====================

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : length(0)
{
    head = new Node(); // dummy head
    tail = new Node(); // dummy tail
    head->next = tail;
    tail->prev = head;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    Node* current = head;
    while (current) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    length = 0;
}

// TODO BTL1

template <typename T>
void DoublyLinkedList<T>::insertAtHead(T data) {
    Node* newNode = new Node(data, nullptr, head);
    if (head != nullptr) {
        head->prev = newNode;
    } else {
        tail = newNode;
    }
    head = newNode;
    length++;
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
    length++;
}

template <typename T>
void DoublyLinkedList<T>::insertAt(int index, T data) {
    if (index < 0 || index > length) {
        throw std::out_of_range("Index is invalid!");
    }
    if (index == 0) {
        insertAtHead(data);
        return;
    }
    if (index == length) {
        insertAtTail(data);
        return;
    }
    // traverse to node at index
    Node* current = head;
    for (int i = 0; i < index; ++i) current = current->next;
    Node* newNode = new Node(data, current->prev, current);
    if (current->prev) current->prev->next = newNode;
    current->prev = newNode;
    length++;
}

template <typename T>
void DoublyLinkedList<T>::deleteAt(int index) {
    if (index < 0 || index >= length) {
        throw std::out_of_range("Index is invalid!");
    }
    Node* toDelete = head;
    for (int i = 0; i < index; ++i) toDelete = toDelete->next;
    if (toDelete->prev) toDelete->prev->next = toDelete->next;
    else head = toDelete->next;
    if (toDelete->next) toDelete->next->prev = toDelete->prev;
    else tail = toDelete->prev;
    delete toDelete;
    length--;
}

// const-get
template <typename T>
T &DoublyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= length) throw std::out_of_range("Index is invalid!");
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
int DoublyLinkedList<T>::size() const { return length; }

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
template<class T> std::string DoublyLinkedList<T>::toString(std::string (*convert2str)(T &)) const {
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
       
        root = new Node(s.substr(0, CHUNK_SIZE));

      
        int start = CHUNK_SIZE;
        while (start < s.size()) {
            int chunkSize = (len - start > CHUNK_SIZE) ? CHUNK_SIZE : (len - start);
            Node* newNode = new Node(s.substr(start, chunkSize));
            root = concatNodes(root, newNode); 
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


// ==================== ROPETEXTBUFFER IMPLEMENTATION ====================
RopeTextBuffer::RopeTextBuffer()
    : cursorPos(0), rope(), history(nullptr) 
{
    history = new HistoryManager();
}

RopeTextBuffer::~RopeTextBuffer()
{
    delete history;
    history = nullptr;
}

void RopeTextBuffer::insert(const string &s)
{
    if (s.empty()) return;

    int before = cursorPos;

    rope.insert(cursorPos, s);
    cursorPos += static_cast<int>(s.size());

    HistoryManager::Action a;
    a.actionName = "insert";
    a.cursorBefore = before;
    a.cursorAfter  = cursorPos;
    a.data = s;
    history->addAction(a);
}

void RopeTextBuffer::deleteRange(int length)
{
    if (length < 0) throw std::out_of_range("Length is invalid!");
    if (length == 0) return;

 
    if (cursorPos + length > rope.length())
        throw std::out_of_range("Length is invalid!");

 
    string removed = rope.substring(cursorPos, length);

   
    rope.deleteRange(cursorPos, length);


    HistoryManager::Action a;
    a.actionName = "delete";
    a.cursorBefore = cursorPos;
    a.cursorAfter  = cursorPos;
    a.data = removed;
    history->addAction(a);
}

void RopeTextBuffer::replace(int length, const string &s)
{
    
    if (length < 0) throw std::out_of_range("Length is invalid!");
    if (cursorPos + length > rope.length())
        throw std::out_of_range("Length is invalid!");

    int before = cursorPos;

    string oldStr = (length > 0) ? rope.substring(cursorPos, length) : string();

 
    if (length > 0) rope.deleteRange(cursorPos, length);
    if (!s.empty()) {
        rope.insert(cursorPos, s);
        cursorPos += static_cast<int>(s.size());
    }

    HistoryManager::Action a;
    a.actionName = "replace";
    a.cursorBefore = before;
    a.cursorAfter  = cursorPos; 
    a.data = oldStr;            
    history->addAction(a);
}

void RopeTextBuffer::moveCursorTo(int index)
{
    
    if (index < 0 || index > rope.length())
        throw std::out_of_range("Index is invalid!");

    int before = cursorPos;
    cursorPos = index;

    HistoryManager::Action a;
    a.actionName = "move";
    a.cursorBefore = before;
    a.cursorAfter  = cursorPos;
    a.data = "J"; // Jump
    history->addAction(a);
}

void RopeTextBuffer::moveCursorLeft()
{
    
    if (cursorPos <= 0)
        throw  cursor_error();

    int before = cursorPos;
    cursorPos--;

    HistoryManager::Action a;
    a.actionName = "move";
    a.cursorBefore = before;
    a.cursorAfter  = cursorPos;
    a.data = "L";
    history->addAction(a);
}

void RopeTextBuffer::moveCursorRight()
{
    
    if (cursorPos >= rope.length())
        throw cursor_error();

    int before = cursorPos;
    cursorPos++;

    HistoryManager::Action a;
    a.actionName = "move";
    a.cursorBefore = before;
    a.cursorAfter  = cursorPos;
    a.data = "R";
    history->addAction(a);
}

int RopeTextBuffer::getCursorPos() const
{
   
    return cursorPos;
}

string RopeTextBuffer::getContent() const
{
    
    return rope.toString();
}

int RopeTextBuffer::findFirst(char c) const
{
    
    string s = rope.toString();
    size_t pos = s.find(c);
    if (pos == string::npos) return -1;
    return static_cast<int>(pos);
}

int *RopeTextBuffer::findAll(char c) const
{
   
    string s = rope.toString();
    int len = static_cast<int>(s.size());

   
    int count = 0;
    for (int i = 0; i < len; ++i) {
        if (s[i] == c) count++;
    }
    if (count == 0) return nullptr;

  
    int *arr = new int[count + 1];
    int idx = 0;

   
    for (int i = 0; i < len; ++i) {
        if (s[i] == c) {
            arr[idx++] = i;
        }
    }

    
    arr[count] = -1;
    return arr;
}

void RopeTextBuffer::printHistory() const {
    if (history) {
        history->printHistory();
    } else {
        std::cout << "[]" << std::endl; // Không có lịch sử nào
    }
}



void RopeTextBuffer::clear()
{
  
    rope = Rope();     
    cursorPos = 0;    


    delete history;
    history = new HistoryManager();
}

// ==================== HistoryManager ====================

HistoryManager::HistoryManager() {
    currentIndex = 0;
    current = actions.end();  // iterator trỏ tới tail (list rỗng thì head->next = tail)
}


HistoryManager::~HistoryManager() {

}

void HistoryManager::addAction(const Action &a) {

    auto it = actions.begin();
    while (it != actions.end() && it != current) {
        ++it;
    }

    while (it != actions.end()) {
        actions.deleteAt(actions.size() - 1); 

        it = actions.end();
    }

    actions.insertAtTail(a);

    current = --actions.end();   
}




bool HistoryManager::canUndo() const {
    return current != actions.begin();
}

bool HistoryManager::canRedo() const {
    auto it = current;
    ++it; 
    return it != actions.end();
}

// void HistoryManager::printHistory() const
// {
//     cout << "[";
//     for (int i = 0; i < actions.size(); ++i) {
//         const Action &a = actions.get(i);
//         if (i > 0) cout << ", ";
//         cout << "("
//              << a.actionName << ", "
//              << a.cursorBefore << ", "
//              << a.cursorAfter << ", "
//              << a.data
//              << ")";
//     }
//     cout << "]" << endl;
// }

void HistoryManager::printHistory() const {
    cout << "[";
    bool first = true;
    auto it_temp = actions.begin();  
if (it_temp != actions.end()) {  
    cout << (*it_temp).data << endl;  
}

    for (auto it = actions.begin(); it != actions.end(); ++it) {
        if (!first) cout << ", ";
        const Action &a = *it;
        cout << "("
             << a.actionName << ", "
             << a.cursorBefore << ", "
             << a.cursorAfter << ", "
             << a.data << ")";
        first = false;
    }
    cout << "]" << endl;
}


void RopeTextBuffer::undo() {}

void RopeTextBuffer::redo() {}

