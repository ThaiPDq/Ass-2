#include "RopeTextBuffer.h"

// ==================== DoublyLinkedList IMPLEMENTATION ====================

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : length(0)
{
    head = new Node(); // dummy head
    tail = new Node(); // dummy tail
    head->next = tail;
    tail->prev = head;
}
// TODO BTL1

// ==================== NODE IMPLEMENTATION ====================

Rope::Node::Node() : left(nullptr), right(nullptr), data(""), weight(0), height(1), balance(EH) {}

// TODO TASK 1

// ==================== ROPE IMPLEMENTATION ====================

Rope::Rope() : root(nullptr) {}

// TODO TASK 1

// ==================== PUBLIC INTERFACE ====================

int Rope::length() const
{
    return getTotalLength(root);
}
// TODO TASK 1

// ==================== ROPETEXTBUFFER IMPLEMENTATION ====================
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

// ==================== HistoryManager ====================
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