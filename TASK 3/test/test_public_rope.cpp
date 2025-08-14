#include "TestHelper.h"

void TestHelper::test_public_Basic()
{
    Rope::Node *root = TestHelper::createInternalNode(
        TestHelper::createInternalNode(
            TestHelper::createLeafNode("Hello_"),
            TestHelper::createLeafNode("my_"),
            6, 2, Rope::Node::BalanceFactor::EH // weight = 6, height = 2
            ),
        TestHelper::createInternalNode(
            TestHelper::createInternalNode(
                TestHelper::createLeafNode("na"),
                TestHelper::createLeafNode("me_i"),
                2, 2, Rope::Node::BalanceFactor::EH // weight = 2, height = 2
                ),
            TestHelper::createInternalNode(
                TestHelper::createLeafNode("s"),
                TestHelper::createLeafNode("_DSA"),
                1, 2, Rope::Node::BalanceFactor::EH // weight = 1, height = 2
                ),
            6, 3, Rope::Node::BalanceFactor::EH // weight = 6, height = 3
            ),
        9, 4, Rope::Node::BalanceFactor::RH // root: weight = 9, height = 4
    );

    Rope *test = TestHelper::createRope(root);

    EXPECT_EQ(test->toString(), "Hello_my_name_is_DSA");
    EXPECT_EQ(test->length(), 20);
    EXPECT_EQ(test->empty(), false);
    EXPECT_EQ(test->charAt(0), 'H');
    EXPECT_EQ(test->charAt(19), 'A');
    EXPECT_EQ(test->charAt(10), 'a');
    EXPECT_EQ(test->substring(7, 11), "y_name_is_D");
    delete test;
}

void TestHelper::test_public_insert_null()
{
    Rope test;
    try
    {
        test.insert(1, "ABC");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
    test.insert(0, "1234567890123456789012345");
    EXPECT_EQ(test.toString(), "1234567890123456789012345");
    auto expected = createInternalNode(
        createInternalNode(createLeafNode("12345678"), createLeafNode("90123456"), 8, 2, Rope::Node::EH),
        createInternalNode(createLeafNode("78901234"), createLeafNode("5"), 8, 2, Rope::Node::EH),
        16, 3, Rope::Node::EH);
    EXPECT_EQ(nodeToString(test.root), nodeToString(expected));
    test.destroy(expected);
}

void TestHelper::test_public_insert()
{
    Rope::Node *root = TestHelper::createInternalNode(
        TestHelper::createLeafNode("Hello"),
        TestHelper::createLeafNode("d"),
        5, 2, Rope::Node::BalanceFactor::EH);

    Rope *test = TestHelper::createRope(root);
    EXPECT_EQ(test->toString(), "Hellod");
    test->insert(3, "_DataStructure");
    EXPECT_EQ(test->toString(), "Hel_DataStructurelod");
    delete test;
}

void TestHelper::test_public_deleteRange()
{
    Rope test;
    test.insert(0, "Hello_World");

    EXPECT_EQ(test.toString(), "Hello_World");
    test.deleteRange(5, 5);
    auto expected = createInternalNode(
        createLeafNode("Hello"),
        createLeafNode("d"),
        5, 2, Rope::Node::EH);
    EXPECT_EQ(nodeToString(test.root), nodeToString(expected));
    test.destroy(expected);
}