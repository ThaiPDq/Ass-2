#include "TestHelper.h"

void TestHelper::test_private_Basic()
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

    Rope test;
    EXPECT_EQ(test.height(root), 4);
    EXPECT_EQ(test.height(root->left->left), 1);
    EXPECT_EQ(test.height(root->right->left), 2);

    EXPECT_EQ(test.getTotalLength(root), 20);
    EXPECT_EQ(test.getTotalLength(root->right), 11);

    EXPECT_EQ(test.charAt(root, 0), 'H');
    EXPECT_EQ(test.charAt(root, 19), 'A');
    EXPECT_EQ(test.charAt(root, 10), 'a');

    EXPECT_EQ(test.toString(root), "Hello_my_name_is_DSA");

    root->balance = Rope::Node::BalanceFactor::EH;
    root->height = 0;
    root->weight = 0;
    test.update(root);
    EXPECT_EQ(root->balance, Rope::Node::BalanceFactor::RH);
    EXPECT_EQ(root->height, 4);
    EXPECT_EQ(root->weight, 9);

    test.destroy(root);
    EXPECT_EQ(test.toString(root), "");
}

void TestHelper::test_private_rotateLeft()
{
    Rope rope;

    auto A = createLeafNode("A");
    auto B = createLeafNode("B");
    auto C = createLeafNode("C");

    auto left = createInternalNode(A, B, 1, 2, Rope::Node::EH);
    auto root = createInternalNode(left, C, 2, 3, Rope::Node::LH);

    Rope::Node *actual = rope.rotateRight(root);

    auto expected = createInternalNode(
        createLeafNode("A"),
        createInternalNode(createLeafNode("B"), createLeafNode("C"), 1, 2, Rope::Node::EH),
        1, 3, Rope::Node::RH);

    EXPECT_EQ(nodeToString(actual), nodeToString(expected));
    rope.destroy(expected);
    rope.destroy(actual);
}

void TestHelper::test_private_rotateRight()
{
    Rope rope;

    auto A = createLeafNode("A");
    auto B = createLeafNode("B");
    auto C = createLeafNode("C");

    auto right = createInternalNode(B, C, 1, 2, Rope::Node::EH);
    auto root = createInternalNode(A, right, 1, 3, Rope::Node::RH);

    Rope::Node *actual = rope.rotateLeft(root);

    auto expected = createInternalNode(
        createInternalNode(createLeafNode("A"), createLeafNode("B"), 1, 2, Rope::Node::EH),
        createLeafNode("C"),
        2, 3, Rope::Node::LH);

    EXPECT_EQ(nodeToString(actual), nodeToString(expected));
    rope.destroy(expected);
    rope.destroy(actual);
}

void TestHelper::test_private_LL()
{
    Rope rope;

    auto A = createLeafNode("A");
    auto B = createLeafNode("B");
    auto C = createLeafNode("C");
    auto D = createLeafNode("D");
    auto AB = createInternalNode(A, B, 1, 2, Rope::Node::EH);
    auto ABC = createInternalNode(AB, C, 2, 3, Rope::Node::LH);
    auto ABCD = createInternalNode(ABC, D, 3, 4, Rope::Node::LH);

    Rope::Node *actual = rope.rebalance(ABCD);

    auto expected = createInternalNode(
        createInternalNode(createLeafNode("A"), createLeafNode("B"), 1, 2, Rope::Node::EH),
        createInternalNode(createLeafNode("C"), createLeafNode("D"), 1, 2, Rope::Node::EH),
        2, 3, Rope::Node::EH);

    EXPECT_EQ(nodeToString(actual), nodeToString(expected));
    rope.destroy(actual);
    rope.destroy(expected);
}

void TestHelper::test_private_RR()
{
    Rope rope;

    auto A = createLeafNode("A");
    auto B = createLeafNode("B");
    auto C = createLeafNode("C");
    auto D = createLeafNode("D");
    auto CD = createInternalNode(C, D, 1, 2, Rope::Node::EH);
    auto BCD = createInternalNode(B, CD, 1, 3, Rope::Node::RH);
    auto ABCD = createInternalNode(A, BCD, 1, 4, Rope::Node::RH);

    Rope::Node *actual = rope.rebalance(ABCD);

    auto expected = createInternalNode(
        createInternalNode(createLeafNode("A"), createLeafNode("B"), 1, 2, Rope::Node::EH),
        createInternalNode(createLeafNode("C"), createLeafNode("D"), 1, 2, Rope::Node::EH),
        2, 3, Rope::Node::EH);

    EXPECT_EQ(nodeToString(actual), nodeToString(expected));
    rope.destroy(actual);
    rope.destroy(expected);
}

void TestHelper::test_private_LR()
{
    Rope rope;

    auto A = createLeafNode("A");
    auto B = createLeafNode("B");
    auto C = createLeafNode("C");
    auto D = createLeafNode("D");
    auto AB = createInternalNode(A, B, 1, 2, Rope::Node::EH);
    auto ABC = createInternalNode(C, AB, 1, 3, Rope::Node::RH);
    auto ABCD = createInternalNode(ABC, D, 3, 4, Rope::Node::LH);

    Rope::Node *actual = rope.rebalance(ABCD);

    auto expected = createInternalNode(
        createInternalNode(createLeafNode("C"), createLeafNode("A"), 1, 2, Rope::Node::EH),
        createInternalNode(createLeafNode("B"), createLeafNode("D"), 1, 2, Rope::Node::EH),
        2, 3, Rope::Node::EH);

    EXPECT_EQ(nodeToString(actual), nodeToString(expected));
    rope.destroy(actual);
    rope.destroy(expected);
}

void TestHelper::test_private_RL()
{
    Rope rope;

    auto A = createLeafNode("A");
    auto B = createLeafNode("B");
    auto C = createLeafNode("C");
    auto D = createLeafNode("D");
    auto CD = createInternalNode(C, D, 1, 2, Rope::Node::EH);
    auto BCD = createInternalNode(CD, B, 2, 3, Rope::Node::LH);
    auto ABCD = createInternalNode(A, BCD, 1, 4, Rope::Node::RH);

    Rope::Node *actual = rope.rebalance(ABCD);

    auto expected = createInternalNode(
        createInternalNode(createLeafNode("A"), createLeafNode("C"), 1, 2, Rope::Node::EH),
        createInternalNode(createLeafNode("D"), createLeafNode("B"), 1, 2, Rope::Node::EH),
        2, 3, Rope::Node::EH);

    EXPECT_EQ(nodeToString(actual), nodeToString(expected));
    rope.destroy(actual);
    rope.destroy(expected);
}

void TestHelper::test_private_Conat()
{
    Rope rope;

    auto A = createLeafNode("A");
    auto B = createLeafNode("B");
    auto C = createLeafNode("C");
    auto D = createLeafNode("D");
    auto CD = createInternalNode(C, D, 1, 2, Rope::Node::EH);
    auto BCD = createInternalNode(CD, B, 2, 3, Rope::Node::LH);
    auto concat = rope.concatNodes(A, BCD);

    auto expected = createInternalNode(
        createInternalNode(createLeafNode("A"), createLeafNode("C"), 1, 2, Rope::Node::EH),
        createInternalNode(createLeafNode("D"), createLeafNode("B"), 1, 2, Rope::Node::EH),
        2, 3, Rope::Node::EH);

    EXPECT_EQ(nodeToString(concat), nodeToString(expected));
    rope.destroy(concat);
    rope.destroy(expected);
}

void TestHelper::test_private_Split()
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

    Rope test;
    Rope::Node *left;
    Rope::Node *right;
    test.split(root, 10, left, right);
    auto expected_left = createInternalNode(
        createInternalNode(createLeafNode("Hello_"), createLeafNode("my_"), 6, 2, Rope::Node::EH),
        createLeafNode("n"),
        9, 3, Rope::Node::LH);

    auto expected_right = createInternalNode(
        createInternalNode(createLeafNode("a"), createLeafNode("me_i"), 1, 2, Rope::Node::EH),
        createInternalNode(createLeafNode("s"), createLeafNode("_DSA"), 1, 2, Rope::Node::EH),
        5, 3, Rope::Node::EH);
    EXPECT_EQ(nodeToString(left), nodeToString(expected_left));
    EXPECT_EQ(nodeToString(right), nodeToString(expected_right));
    test.destroy(left);
    test.destroy(right);
    test.destroy(expected_left);
    test.destroy(expected_right);
}