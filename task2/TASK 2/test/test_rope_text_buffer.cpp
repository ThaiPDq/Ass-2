#include "TestHelper.h"

void TestHelper::test_rope_text_buffer_insert()
{
    RopeTextBuffer buffer;

    EXPECT_EQ(buffer.getContent(), "");
    EXPECT_EQ(buffer.getCursorPos(), 0);

    buffer.insert("Hello");
    EXPECT_EQ(buffer.getContent(), "Hello");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.moveCursorTo(2);
    buffer.insert("_X_");
    EXPECT_EQ(buffer.getContent(), "He_X_llo");
    EXPECT_EQ(buffer.getCursorPos(), 5);

    buffer.moveCursorTo(0);
    buffer.insert("START_");
    EXPECT_EQ(buffer.getContent(), "START_He_X_llo");
    EXPECT_EQ(buffer.getCursorPos(), 6);

    buffer.moveCursorTo(buffer.getContent().size());
    buffer.insert("_END");
    EXPECT_EQ(buffer.getContent(), "START_He_X_llo_END");
}

void TestHelper::test_rope_text_buffer_move()
{
    RopeTextBuffer buffer;
    buffer.insert("Hello_my_name_is_DSA");
    EXPECT_EQ(buffer.getContent(), "Hello_my_name_is_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 20);
    try
    {
        buffer.moveCursorRight();
    }
    catch (const cursor_error &e)
    {
        EXPECT_EQ(std::string(e.what()), "Cursor error!");
    }
    buffer.moveCursorTo(0);
    EXPECT_EQ(buffer.getCursorPos(), 0);
    try
    {
        buffer.moveCursorLeft();
    }
    catch (const cursor_error &e)
    {
        EXPECT_EQ(std::string(e.what()), "Cursor error!");
    }

    buffer.moveCursorRight();
    buffer.moveCursorRight();
    buffer.moveCursorRight();
    buffer.moveCursorRight();
    buffer.moveCursorRight();
    EXPECT_EQ(buffer.getCursorPos(), 5);
    buffer.moveCursorLeft();
    buffer.moveCursorLeft();
    buffer.moveCursorLeft();
    EXPECT_EQ(buffer.getCursorPos(), 2);

    try
    {
        buffer.moveCursorTo(21);
    }
    catch (const out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
    EXPECT_EQ(buffer.getCursorPos(), 2);
    buffer.moveCursorTo(10);
    EXPECT_EQ(buffer.getCursorPos(), 10);
    EXPECT_EQ(buffer.getContent(), "Hello_my_name_is_DSA");
    buffer.clear();
    EXPECT_EQ(buffer.getCursorPos(), 0);
    EXPECT_EQ(buffer.getContent(), "");
}

void TestHelper::test_rope_text_buffer_deleteRange()
{
    RopeTextBuffer buffer;

    buffer.insert("Hello_my_name_is_DSA");
    EXPECT_EQ(buffer.getContent(), "Hello_my_name_is_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 20);

    buffer.moveCursorTo(6);
    buffer.deleteRange(3);
    EXPECT_EQ(buffer.getContent(), "Hello_name_is_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 6);

    buffer.deleteRange(5);
    EXPECT_EQ(buffer.getContent(), "Hello_is_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 6);

    int remaining = (int)buffer.getContent().size() - buffer.getCursorPos();
    buffer.deleteRange(remaining);
    EXPECT_EQ(buffer.getContent(), "Hello_");
    EXPECT_EQ(buffer.getCursorPos(), 6);

    try
    {
        buffer.deleteRange(10);
    }
    catch (const out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Length is invalid!");
    }
    EXPECT_EQ(buffer.getContent(), "Hello_");
    EXPECT_EQ(buffer.getCursorPos(), 6);

    buffer.moveCursorTo(0);
    buffer.deleteRange(6);
    EXPECT_EQ(buffer.getContent(), "");
    EXPECT_EQ(buffer.getCursorPos(), 0);
}

void TestHelper::test_rope_text_buffer_replace()
{
    RopeTextBuffer buffer;

    buffer.insert("Hello_my_name_is_DSA");
    EXPECT_EQ(buffer.getContent(), "Hello_my_name_is_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 20);

    buffer.moveCursorTo(5);
    buffer.replace(3, "_your");
    EXPECT_EQ(buffer.getContent(), "Hello_your_name_is_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 10);

    buffer.replace(5, "_friend");
    EXPECT_EQ(buffer.getContent(), "Hello_your_friend_is_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 17);

    buffer.moveCursorRight();
    buffer.replace(3, "");
    EXPECT_EQ(buffer.getContent(), "Hello_your_friend_DSA");
    EXPECT_EQ(buffer.getCursorPos(), 18);

    buffer.replace(3, "CS");
    EXPECT_EQ(buffer.getContent(), "Hello_your_friend_CS");
    EXPECT_EQ(buffer.getCursorPos(), 20);

    try
    {
        buffer.replace(100, "test");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Length is invalid!");
    }
}

void TestHelper::test_rope_text_buffer_find()
{
    RopeTextBuffer buffer;

    buffer.insert("abracadabra");
    EXPECT_EQ(buffer.getContent(), "abracadabra");

    int pos = buffer.findFirst('a');
    EXPECT_EQ(pos, 0);

    pos = buffer.findFirst('z');
    EXPECT_EQ(pos, -1);

    int *positions = buffer.findAll('a');
    int expectedA[] = {0, 3, 5, 7, 10};
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(positions[i], expectedA[i]);
    }
    delete[] positions;

    positions = buffer.findAll('z');
    EXPECT_EQ(positions, nullptr);
}

void TestHelper::test_rope_text_buffer_full_sequence()
{
    RopeTextBuffer buffer;

    // Ban đầu rỗng, cursor tại 0
    EXPECT_EQ(buffer.getContent(), "");
    EXPECT_EQ(buffer.getCursorPos(), 0);

    // Thao tác 1: insert("A")
    buffer.insert("A");
    EXPECT_EQ(buffer.getContent(), "A");
    EXPECT_EQ(buffer.getCursorPos(), 1);

    // Thao tác 2: insert("CSE")
    buffer.insert("CSE");
    EXPECT_EQ(buffer.getContent(), "ACSE");
    EXPECT_EQ(buffer.getCursorPos(), 4);

    // Thao tác 3: insert("HCMUT")
    buffer.insert("HCMUT");
    EXPECT_EQ(buffer.getContent(), "ACSEHCMUT");
    EXPECT_EQ(buffer.getCursorPos(), 9);

    // Thao tác 4: moveCursorLeft()
    buffer.moveCursorLeft();
    EXPECT_EQ(buffer.getCursorPos(), 8);
    EXPECT_EQ(buffer.getContent(), "ACSEHCMUT");

    // Thao tác 5: insert("123")
    buffer.insert("123");
    EXPECT_EQ(buffer.getContent(), "ACSEHCMU123T");
    EXPECT_EQ(buffer.getCursorPos(), 11);

    // Thao tác 6: moveCursorTo(4)
    buffer.moveCursorTo(4);
    EXPECT_EQ(buffer.getCursorPos(), 4);
    EXPECT_EQ(buffer.getContent(), "ACSEHCMU123T");

    // Thao tác 7: deleteRange(3)
    buffer.deleteRange(3);
    EXPECT_EQ(buffer.getContent(), "ACSEU123T");
    EXPECT_EQ(buffer.getCursorPos(), 4);

    // Thao tác 8: undo() (khôi phục đoạn xóa)
    buffer.undo();

    // Thao tác 9: undo() (di chuyển cursor trở lại vị trí trước thao tác 6)
    buffer.undo();

    // Thao tác 10: undo() (xóa chuỗi "123" đã thêm)
    buffer.undo();

    // Thao tác 11: redo() (thêm lại chuỗi "123")
    buffer.redo();

    // Thao tác 12: redo() (di chuyển cursor về lại vị trí 4)
    buffer.redo();

    // Thao tác 13: redo() (xóa 3 ký tự)
    buffer.redo();
}
