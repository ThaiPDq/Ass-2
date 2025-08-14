#include "TestHelper.h"

void TestHelper::test_undo()
{
    RopeTextBuffer buffer;

    // 1. Insert "Hello"
    buffer.insert("Hello");
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    // 2. Insert " World"
    buffer.insert(" World");
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    // 3. Delete " World"
    buffer.moveCursorTo(5);
    buffer.deleteRange(6); // xóa " World"
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.undo();
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.undo();
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    buffer.undo();
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.moveCursorTo(2);
    buffer.moveCursorLeft();
    buffer.moveCursorRight();
    EXPECT_EQ(buffer.getCursorPos(), 2);

    buffer.undo();
    EXPECT_EQ(buffer.getCursorPos(), 1);

    buffer.undo();
    EXPECT_EQ(buffer.getCursorPos(), 2);

    buffer.replace(3, "LLO");
    EXPECT_EQ(buffer.getContent(), "HeLLO");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.undo();
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 2);

    {
        std::stringstream b;
        std::streambuf *old = std::cout.rdbuf(b.rdbuf());
        buffer.printHistory();
        std::cout.rdbuf(old);
        std::string expected = "[(insert, 0, 5, Hello), (move, 5, 2, J)]";
        EXPECT_EQ(b.str(), expected);
        EXPECT_EQ(buffer.history->canRedo(), true);
    }

    buffer.insert("Hello");
    EXPECT_EQ(buffer.getContent(), "HeHellollo");
    EXPECT_EQ(buffer.getCursorPos(), 7);

    {
        std::stringstream b;
        std::streambuf *old = std::cout.rdbuf(b.rdbuf());
        buffer.printHistory();
        std::cout.rdbuf(old);
        std::string expected = "[(insert, 0, 5, Hello), (move, 5, 2, J), (insert, 2, 7, Hello)]";
        EXPECT_EQ(b.str(), expected);
        EXPECT_EQ(buffer.history->canRedo(), false);
    }
}

void TestHelper::test_redo()
{
    RopeTextBuffer buffer;

    // 1. Insert "Hello"
    buffer.insert("Hello");
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    // 2. Insert " World"
    buffer.insert(" World");
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    // 3. Delete " World"
    buffer.moveCursorTo(5);
    buffer.deleteRange(6);
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    // --- Undo toàn bộ ---
    buffer.undo(); // undo delete -> quay lại "Hello World"
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.undo(); // undo moveCursorTo
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    buffer.undo(); // undo insert " World" -> "Hello"
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.undo(); // undo insert "Hello" -> ""
    EXPECT_EQ(buffer.getContent(), "");
    EXPECT_EQ(buffer.getCursorPos(), 0);

    // --- Redo từng bước ---
    buffer.redo(); // redo insert "Hello"
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.redo(); // redo insert " World"
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    buffer.redo(); // redo
    EXPECT_EQ(buffer.getContent(), "Hello World");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    {
        std::stringstream b;
        std::streambuf *old = std::cout.rdbuf(b.rdbuf());
        buffer.printHistory();
        std::cout.rdbuf(old);
        std::string expected = "[(insert, 0, 5, Hello), (insert, 5, 11,  World), (move, 11, 5, J)]";
        EXPECT_EQ(b.str(), expected);
    }

    buffer.redo(); // redo delete " World"
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    // --- Test move cursor redo ---
    buffer.moveCursorTo(2);
    buffer.moveCursorLeft();  // cursor 1
    buffer.moveCursorRight(); // cursor 2
    EXPECT_EQ(buffer.getCursorPos(), 2);

    buffer.undo(); // undo move right -> cursor 1
    EXPECT_EQ(buffer.getCursorPos(), 1);

    buffer.undo(); // undo move left -> cursor 2
    EXPECT_EQ(buffer.getCursorPos(), 2);

    buffer.redo(); // redo move left -> cursor 1
    EXPECT_EQ(buffer.getCursorPos(), 1);

    buffer.redo(); // redo move right -> cursor 2
    EXPECT_EQ(buffer.getCursorPos(), 2);

    // --- Test replace redo ---
    buffer.replace(3, "LLO1"); // HeLLO
    EXPECT_EQ(buffer.getContent(), "HeLLO1");
    EXPECT_EQ(buffer.getCursorPos(), 6);

    buffer.undo(); // quay lại Hello
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 2);

    buffer.redo(); // redo replace
    EXPECT_EQ(buffer.getContent(), "HeLLO1");
    EXPECT_EQ(buffer.getCursorPos(), 6);

    {
        std::stringstream b;
        std::streambuf *old = std::cout.rdbuf(b.rdbuf());
        buffer.printHistory();
        std::cout.rdbuf(old);
        std::string expected = "[(insert, 0, 5, Hello), (insert, 5, 11,  World), (move, 11, 5, J), (delete, 5, 5,  World), (move, 5, 2, J), (move, 2, 1, L), (move, 1, 2, R), (replace, 2, 6, llo)]";
        EXPECT_EQ(b.str(), expected);
    }
}

void TestHelper::test_full()
{
    RopeTextBuffer buffer;

    buffer.insert("A");
    buffer.insert("CSE");
    buffer.insert("HCMUT");
    buffer.moveCursorLeft();
    buffer.insert("123");
    buffer.moveCursorTo(4);
    buffer.deleteRange(3);
    EXPECT_EQ(buffer.getContent(), "ACSEU123T");
    EXPECT_EQ(buffer.getCursorPos(), 4);

    // 8: undo() -> khôi phục đoạn xóa (trở lại "ACSEHCMU123T")
    buffer.undo();
    EXPECT_EQ(buffer.getContent(), "ACSEHCMU123T");
    EXPECT_EQ(buffer.getCursorPos(), 4);

    // 9: undo() -> quay lại vị trí con trỏ trước thao tác moveCursorTo(4)
    buffer.undo();
    EXPECT_EQ(buffer.getContent(), "ACSEHCMU123T");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    // 10: undo() -> xóa chuỗi "123" đã chèn ở bước 5
    buffer.undo();
    EXPECT_EQ(buffer.getContent(), "ACSEHCMUT");
    EXPECT_EQ(buffer.getCursorPos(), 8);

    // 11: redo() -> thêm lại chuỗi "123" vào vị trí cursor=8
    buffer.redo();
    EXPECT_EQ(buffer.getContent(), "ACSEHCMU123T");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    // 12: redo() -> di chuyển con trỏ về vị trí 4
    buffer.redo();
    EXPECT_EQ(buffer.getContent(), "ACSEHCMU123T");
    EXPECT_EQ(buffer.getCursorPos(), 4);

    // 13: redo() -> xóa 3 ký tự tại vị trí 4
    buffer.redo();
    EXPECT_EQ(buffer.getContent(), "ACSEU123T");
    EXPECT_EQ(buffer.getCursorPos(), 4);
}
