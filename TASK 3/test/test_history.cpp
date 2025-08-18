#include "TestHelper.h"

void TestHelper::test_history()
{
    HistoryManager hm;

    {
        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        hm.printHistory();
        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[]"); cout<<"1";
    }

    hm.addAction({"insert", 0, 1, "A"});
    hm.addAction({"delete", 1, 1, "B"});
    hm.addAction({"move", 1, 0, "L"});
    hm.addAction({"move", 0, 1, "R"});
    hm.addAction({"insert", 0, 1, "A"});
    hm.addAction({"move", 1, 5, "J"});
    hm.addAction({"replace", 5, 10, "old_string"});

    {
        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        hm.printHistory();
        std::cout.rdbuf(old);

        std::string expected = "[(insert, 0, 1, A), (delete, 1, 1, B), (move, 1, 0, L), (move, 0, 1, R), (insert, 0, 1, A), (move, 1, 5, J), (replace, 5, 10, old_string)]";

        EXPECT_EQ(buffer.str(), expected); cout<<"2";
    }
}

void TestHelper::test_rop_text_buffer_history()
{
    RopeTextBuffer ropeTextBuffer;

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), false);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), false);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[]");
    }

    ropeTextBuffer.insert("A");
    ropeTextBuffer.insert("CSE");
    ropeTextBuffer.insert("HCMUT");
    ropeTextBuffer.moveCursorLeft();
    ropeTextBuffer.insert("123");
    ropeTextBuffer.moveCursorTo(4);
    ropeTextBuffer.deleteRange(3);
    EXPECT_EQ(ropeTextBuffer.getContent(), "ACSEU123T");
    EXPECT_EQ(ropeTextBuffer.getCursorPos(), 4);

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), false);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), true);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L), (insert, 8, 11, 123), (move, 11, 4, J), (delete, 4, 4, HCM)]");
    }

    ropeTextBuffer.undo();

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), true);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), true);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L), (insert, 8, 11, 123), (move, 11, 4, J)]");
    }

    ropeTextBuffer.undo();

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), true);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), true);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L), (insert, 8, 11, 123)]");
    }

    ropeTextBuffer.undo();

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), true);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), true);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L)]");
    }

    ropeTextBuffer.redo();

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), true);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), true);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L), (insert, 8, 11, 123)]");
    }

    ropeTextBuffer.redo();

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), true);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), true);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L), (insert, 8, 11, 123), (move, 11, 4, J)]");
    }

    ropeTextBuffer.redo();

    {
        EXPECT_EQ(ropeTextBuffer.history->canRedo(), false);
        EXPECT_EQ(ropeTextBuffer.history->canUndo(), true);

        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
        ropeTextBuffer.printHistory();

        std::cout.rdbuf(old);
        EXPECT_EQ(buffer.str(), "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L), (insert, 8, 11, 123), (move, 11, 4, J), (delete, 4, 4, HCM)]");
    }
}
