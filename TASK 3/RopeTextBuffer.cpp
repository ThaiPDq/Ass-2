#include "RopeTextBuffer.h"

// TODO DoublyLinkedList, Rope, RopeTextBuffer

void RopeTextBuffer::undo()
{
    if (!history->canUndo())
        return;
    HistoryManager::Action a = history->getUndo();

    //! chú ý replace sẽ dùng data_replace and data
}

void RopeTextBuffer::redo()
{
    if (!history->canRedo())
        return;
    HistoryManager::Action a = history->getRedo();

    //! chú ý replace sẽ dùng data_replace and data
}

// ==================== HistoryManager ====================
HistoryManager::HistoryManager() : currentIndex(0) {}

HistoryManager::~HistoryManager() {}

void HistoryManager::addAction(const Action &a)
{
    /* // TODO
       Thêm một hành động mới vào lịch sử:
       - Chèn action tại vị trí currentIndex (để hỗ trợ undo/redo)
       - Nếu action là "insert" hoặc "remove" (thay đổi nội dung),
         xóa toàn bộ các action ở "tương lai" (redo history) vì không còn hợp lệ.
       - Cập nhật currentIndex trỏ về cuối danh sách sau khi xóa.
    */
}

// TODO HistoryManager