#include <gtest/gtest.h>

#define KJUT_ARRAY_INITIAL_CAPACITY 1

#include <kjut/RingBuffer.h>

#define suiteName tst_ringBuffer


TEST(suiteName, test_pushes_and_pops_within_capacity)
{
    Kjut::RingBuffer<int, 4> rb;

    rb.push(1);
    rb.push(2);
    rb.push(3);

    ASSERT_EQ(rb.size(), 3);
    ASSERT_EQ(1, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 2);
    ASSERT_EQ(2, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 1);
    ASSERT_EQ(3, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 0);
}



TEST(suiteName, test_pushes_and_pops_with_overwrite_policy)
{
    Kjut::RingBuffer<int, 4> rb;

    rb.setOverflowPolicy(Kjut::OverflowPolicy::OverwriteOldestData);
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);
    rb.push(5);
    rb.push(6);
    rb.push(7);
    rb.push(8);
    rb.push(9);

    ASSERT_EQ(rb.size(), 4);
    ASSERT_EQ(6, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 3);
    ASSERT_EQ(7, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 2);
    ASSERT_EQ(8, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 1);
    ASSERT_EQ(9, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 0);
}


TEST(suiteName, test_pushes_and_pops_with_skip_new_data_policy)
{
    Kjut::RingBuffer<int, 4> rb;

    rb.setOverflowPolicy(Kjut::OverflowPolicy::SkipNewData);
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);
    rb.push(5);
    rb.push(6);
    rb.push(7);
    rb.push(8);
    rb.push(9);

    ASSERT_EQ(rb.size(), 4);
    ASSERT_EQ(1, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 3);
    ASSERT_EQ(2, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 2);
    ASSERT_EQ(3, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 1);
    ASSERT_EQ(4, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 0);
}

