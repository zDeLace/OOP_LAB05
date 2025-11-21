#include <gtest/gtest.h>
#include "../include/DoublyLinkedList.h"
#include "../include/FixedMapMemoryResource.h"
#include <memory_resource>
#include <vector>

TEST(DoublyListTest, PushBackAndIteration) {
    FixedMapMemoryResource mem(64, 50);
    DoublyLinkedList<int> list(&mem);

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    std::vector<int> v;
    for (auto x : list)
        v.push_back(x);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(DoublyListTest, PushFrontWorks) {
    FixedMapMemoryResource mem(64, 50);
    DoublyLinkedList<int> list(&mem);

    list.push_front(5);
    list.push_front(3);

    auto it = list.begin();
    ASSERT_NE(it, list.end());
    EXPECT_EQ(*it, 3);
    ++it;
    ASSERT_NE(it, list.end());
    EXPECT_EQ(*it, 5);
}

TEST(DoublyListTest, PopFrontBack) {
    FixedMapMemoryResource mem(64, 50);
    DoublyLinkedList<int> list(&mem);

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_front();  // remove 1
    list.pop_back();   // remove 3

    auto it = list.begin();
    ASSERT_NE(it, list.end());
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(it, list.end());
}

TEST(DoublyListTest, EmptyCheck) {
    FixedMapMemoryResource mem(64, 50);
    DoublyLinkedList<int> list(&mem);

    EXPECT_TRUE(list.empty());

    list.push_back(123);
    EXPECT_FALSE(list.empty());
}

TEST(DoublyListTest, ClearRemovesAll) {
    FixedMapMemoryResource mem(64, 50);
    DoublyLinkedList<int> list(&mem);

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}