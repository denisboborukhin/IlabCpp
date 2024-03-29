#include "parser.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

static int create_big_tests(const std::string &file_name);

static std::string get_local_file(const std::string &file_name)
{
    return (PROJECT_DIR_PATH + std::string {"/tests/tests_files/"} + file_name);
}

int main()
{
    testing::InitGoogleTest();

    return RUN_ALL_TESTS();
}

TEST(cache_perfect, test_1)
{
    int num_hits = get_perfect_hits(slow_get_page_int, get_local_file("perfect_test_1.txt"));
    ASSERT_EQ(num_hits, 13);
}

TEST(cache_perfect, test_2)
{
    int num_hits = get_perfect_hits(slow_get_page_int, get_local_file("perfect_test_2.txt"));
    ASSERT_EQ(num_hits, 810);
}

#if 0
TEST (all_cashes, test_from_cin)
{
    hits hits = get_hits (slow_get_page_int);

    std::cout << "lru hits: " << hits.hits_lru << std::endl;
    std::cout << "lfu hits: " << hits.hits_lfu << std::endl;
    std::cout << "perfect hits: " << hits.hits_perfect << std::endl;
}
#endif

TEST(all_caches, small_test_1)
{
    hits hits = get_hits(slow_get_page_int, get_local_file("small_test_1.txt"));

    int num_lfu_hits = get_lfu_hits(slow_get_page_int, get_local_file("small_test_1.txt"));
    ASSERT_EQ(num_lfu_hits, 5);

    int num_lru_hits = get_lru_hits(slow_get_page_int, get_local_file("small_test_1.txt"));
    ASSERT_EQ(num_lru_hits, 6);

    int num_perfect_hits = get_perfect_hits(slow_get_page_int, get_local_file("small_test_1.txt"));
    ASSERT_EQ(num_perfect_hits, 6);

    ASSERT_EQ(hits.hits_lru, 6);
    ASSERT_EQ(hits.hits_lfu, 5);
    ASSERT_EQ(hits.hits_perfect, 6);
}

TEST(all_caches, small_test_2)
{
    hits hits = get_hits(slow_get_page_int, get_local_file("small_test_2.txt"));

    ASSERT_EQ(hits.hits_lru, 3);
    ASSERT_EQ(hits.hits_lfu, 1);
    ASSERT_EQ(hits.hits_perfect, 3);
}

TEST(all_caches, small_test_3)
{
    hits hits = get_hits(slow_get_page_int, get_local_file("small_test_3.txt"));

    ASSERT_EQ(hits.hits_lru, 2);
    ASSERT_EQ(hits.hits_lfu, 3);
    ASSERT_EQ(hits.hits_perfect, 3);
}

TEST(all_caches, small_test_4)
{
    hits hits = get_hits(slow_get_page_int, get_local_file("small_test_4.txt"));

    ASSERT_EQ(hits.hits_lru, 1);
    ASSERT_EQ(hits.hits_lfu, 4);
    ASSERT_EQ(hits.hits_perfect, 4);
}

TEST(all_caches, small_test_5)
{
    hits hits = get_hits(slow_get_page_int, get_local_file("small_test_5.txt"));

    ASSERT_EQ(hits.hits_lru, 1);
    ASSERT_EQ(hits.hits_lfu, 2);
    ASSERT_EQ(hits.hits_perfect, 4);
}

TEST(all_caches, small_test_6)
{
    hits hits = get_hits(slow_get_page_int, get_local_file("small_test_6.txt"));

    ASSERT_EQ(hits.hits_lru, 6);
    ASSERT_EQ(hits.hits_lfu, 6);
    ASSERT_EQ(hits.hits_perfect, 7);
}

TEST(all_caches, small_test_6_str)
{
    hits hits = get_hits<std::string>(slow_get_page_str, get_local_file("small_test_6.txt"));

    ASSERT_EQ(hits.hits_lru, 6);
    ASSERT_EQ(hits.hits_lfu, 6);
    ASSERT_EQ(hits.hits_perfect, 7);
}

TEST(all_caches, big_trivial_test_1000000)
{
    if (!create_big_tests(get_local_file("big_trivial_test_1000000.txt"))) {
        return;
    }

    hits hits = get_hits(slow_get_page_int, get_local_file("big_trivial_test_1000000.txt"));

    ASSERT_EQ(hits.hits_lru, 0);
    ASSERT_EQ(hits.hits_lfu, 0);
}

int create_big_tests(const std::string &file_path)
{
    std::ifstream check_exist {file_path};
    if (check_exist) {
        check_exist.close();
        return 1;
    }

    check_exist.close();
    std::ofstream tests_stream {file_path};
    if (!tests_stream) {
        std::cout << "error creating test\n";
        return 0;
    }

    std::stringstream test_text {};
    const int capacity = 10;
    test_text << capacity;
    test_text << ' ';
    const int num_keys = 1000000;
    test_text << num_keys;
    test_text << ' ';

    for (int i = 0; i != num_keys; ++i) {
        test_text << i;
        test_text << ' ';
    }

    tests_stream << test_text.rdbuf();
    tests_stream.close();

    return 1;
}
