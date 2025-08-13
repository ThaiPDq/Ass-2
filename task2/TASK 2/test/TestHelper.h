#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "../main.h"
#include "../RopeTextBuffer.h"

class TestHelper
{
public:
    static Rope *createRope(Rope::Node *root)
    {
        Rope *rope = new Rope();
        rope->root = root;
        return rope;
    }

    static Rope::Node *createLeafNode(const std::string &s)
    {
        return new Rope::Node(s);
    }

    static Rope::Node *createInternalNode(Rope::Node *left, Rope::Node *right)
    {
        Rope::Node *node = new Rope::Node();
        node->left = left;
        node->right = right;
        return node;
    }

    static Rope::Node *createInternalNode(Rope::Node *left, Rope::Node *right, int weight, int height, Rope::Node::BalanceFactor balance)
    {
        Rope::Node *node = new Rope::Node();
        node->left = left;
        node->right = right;
        node->weight = weight;
        node->height = height;
        node->balance = balance;
        return node;
    }

    static std::string nodeToString(Rope::Node *node)
    {
        if (!node)
            return "null";

        std::ostringstream oss;

        oss << "{";
        oss << "data:\"" << node->data << "\", ";
        oss << "weight:" << node->weight << ", ";
        oss << "height:" << node->height << ", ";
        oss << "balance:" << balanceToString(node->balance) << ", ";
        oss << "left:" << nodeToString(node->left) << ", ";
        oss << "right:" << nodeToString(node->right);
        oss << "}";

        return oss.str();
    }

    static std::string balanceToString(Rope::Node::BalanceFactor b)
    {
        switch (b)
        {
        case Rope::Node::LH:
            return "LH";
        case Rope::Node::EH:
            return "EH";
        case Rope::Node::RH:
            return "RH";
        default:
            return "Unknown";
        }
    }

    static void test_private_Basic();
    static void test_private_rotateLeft();
    static void test_private_rotateRight();
    static void test_private_LL();
    static void test_private_RR();
    static void test_private_LR();
    static void test_private_RL();
    static void test_private_Conat();
    static void test_private_Split();

    static void test_public_Basic();
    static void test_public_insert_null();
    static void test_public_insert();
    static void test_public_deleteRange();

    static void test_rope_text_buffer_insert();
    static void test_rope_text_buffer_move();
    static void test_rope_text_buffer_deleteRange();
    static void test_rope_text_buffer_replace();
    static void test_rope_text_buffer_find();
    static void test_rope_text_buffer_full_sequence();

    static void test_history();
    static void test_rop_text_buffer_history();
};

#endif // TEST_HELPER_H