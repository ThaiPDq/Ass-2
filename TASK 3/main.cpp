#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <exception>
#include "test/TestHelper.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

//! Build: g++ -std=c++17 -o main main.cpp RopeTextBuffer.cpp test/test_private_rope.cpp test/test_public_rope.cpp
//! Run: ./main all/nameTest

int main(int argc, char *argv[])
{
    std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"test_private_Basic", TestHelper::test_private_Basic},
        {"test_private_rotateLeft", TestHelper::test_private_rotateLeft},
        {"test_private_rotateRight", TestHelper::test_private_rotateRight},
        {"test_private_LL", TestHelper::test_private_LL},
        {"test_private_RR", TestHelper::test_private_RR},
        {"test_private_LR", TestHelper::test_private_LR},
        {"test_private_RL", TestHelper::test_private_RL},
        {"test_private_Conat", TestHelper::test_private_Conat},
        {"test_private_Split", TestHelper::test_private_Split},

        {"test_public_Basic", TestHelper::test_public_Basic},
        {"test_public_insert_null", TestHelper::test_public_insert_null},
        {"test_public_insert", TestHelper::test_public_insert},
        {"test_public_deleteRange", TestHelper::test_public_deleteRange},

        {"test_rope_text_buffer_insert", TestHelper::test_rope_text_buffer_insert},
        {"test_rope_text_buffer_move", TestHelper::test_rope_text_buffer_move},
        {"test_rope_text_buffer_deleteRange", TestHelper::test_rope_text_buffer_deleteRange},
        {"test_rope_text_buffer_replace", TestHelper::test_rope_text_buffer_replace},
        {"test_rope_text_buffer_find", TestHelper::test_rope_text_buffer_find},
        {"test_rope_text_buffer_full_sequence", TestHelper::test_rope_text_buffer_full_sequence},

        {"test_history", TestHelper::test_history},
        {"test_rop_text_buffer_history", TestHelper::test_rop_text_buffer_history},

        {"test_undo", TestHelper::test_undo},
        {"test_redo", TestHelper::test_redo},
        {"test_full", TestHelper::test_full},
    };

    if (argc < 2)
    {
        std::cout << "Usage:\n  ./main all\n  ./main <name>\nAvailable tests:\n";
        for (const auto &[name, _] : tests)
            std::cout << "  - " << name << "\n";
        return 0;
    }

    std::string arg = argv[1];
    bool run_all = (arg == "all");

    for (const auto &[name, func] : tests)
    {
        if (run_all || arg == name)
        {
            std::cout << CYAN << "=== Running test: " << name << " ===" << RESET << "\n";
            try
            {
                func();
                std::cout << GREEN << "✔ PASSED\n"
                          << RESET;
            }
            catch (const std::exception &e)
            {
                std::cerr << RED << "✘ FAILED: " << e.what() << RESET << "\n";
                return 1; // Dừng ngay khi gặp lỗi
            }
        }

        if (!run_all && arg == name)
            break;
    }

    return 0;
}