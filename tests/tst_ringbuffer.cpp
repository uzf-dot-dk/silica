#include <gtest/gtest.h>

#define KJUT_ARRAY_INITIAL_CAPACITY 1

#include <silica/RingBuffer.h>

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




std::vector<std::vector<int>> invocations;

void test_callbacks_with_skip_new_data_policy_callback(const Kjut::RingBuffer<int, 4> &, size_t currentHeadIndex, size_t currentTailIndex, const int& element)
{
    invocations.push_back({(int(currentHeadIndex)), int(currentTailIndex), element});
}


TEST(suiteName, test_callbacks_with_skip_new_data_policy)
{
    Kjut::RingBuffer<int, 4> rb;
    invocations.clear();
    rb.setOverRunCallBack(test_callbacks_with_skip_new_data_policy_callback);
    rb.setOverflowPolicy(Kjut::OverflowPolicy::SkipNewData);
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);
    rb.push(5);
    rb.push(6);

    std::vector<std::vector<int>> expected = {
        {0,4,5},
        {0,4,6}
    };

    ASSERT_EQ(invocations, expected);

}


TEST(suiteName, test_callbacks_with_overwrite_old_data_policy)
{
    Kjut::RingBuffer<int, 4> rb;
    invocations.clear();
    rb.setOverRunCallBack(test_callbacks_with_skip_new_data_policy_callback);
    rb.setOverflowPolicy(Kjut::OverflowPolicy::OverwriteOldestData);
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);
    rb.push(5);
    rb.push(6);

    std::vector<std::vector<int>> expected = {
        {0,4,5},
        {1,0,6}
    };

    ASSERT_EQ(invocations, expected);
}


#define POP_AND_ASSERT_EQ(expected) ASSERT_EQ(int(rb.peek()), expected); ASSERT_TRUE(rb.pop());

TEST(suiteName, test_lots_of_pushes_and_pops_with_overwrite_policy)
{
    Kjut::RingBuffer<int, 4> rb;
    rb.setOverflowPolicy(Kjut::OverflowPolicy::OverwriteOldestData);

    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);

    POP_AND_ASSERT_EQ(1);
    POP_AND_ASSERT_EQ(2);
    POP_AND_ASSERT_EQ(3);
    POP_AND_ASSERT_EQ(4);

    rb.push(5);
    rb.push(6);
    rb.push(7);
    rb.push(8);
    rb.push(9);
    rb.push(10);

    POP_AND_ASSERT_EQ(7);
    POP_AND_ASSERT_EQ(8);
    POP_AND_ASSERT_EQ(9);
    POP_AND_ASSERT_EQ(10);







}


class ThrowingAssignmentOperator
{
public:
    ThrowingAssignmentOperator()
    {
    }

    ThrowingAssignmentOperator(int value, bool shouldThrow)
    {
        this->value = value;
        shallThrowWhenAssignedTo = shouldThrow;
    }

    ThrowingAssignmentOperator(int value)
    {
        this->value = value;
    }

    ThrowingAssignmentOperator & operator=(const ThrowingAssignmentOperator& other)
    {
        const int oldValue = ThrowingAssignmentOperator::value;
        this->value = THIS_IS_UNDEFINED;
        if(shallThrowWhenAssignedTo)
        {
            throw oldValue;
        }
        this->value = other.value;
        this->shallThrowWhenAssignedTo = other.shallThrowWhenAssignedTo;
        return *this;
    }

    explicit operator int() const
    {
        return value;
    }
    mutable bool shallThrowWhenAssignedTo = false;
    int value = -1;

    static constexpr int THIS_IS_UNDEFINED = -31415;

};

std::ostream &operator<<(std::ostream &os, const ThrowingAssignmentOperator &e) {
    os << e.value;
    os << (e.shallThrowWhenAssignedTo ? "T" : "F");
    return os;
}



TEST(suiteName, test_set_integrity_maintained_when_T_destructor_throws_exceptions_during_push)
{
    Kjut::RingBuffer<ThrowingAssignmentOperator, 4> rb ;
    rb.setOverflowPolicy(Kjut::OverflowPolicy::OverwriteOldestData);
    rb.push( 10 );
    rb.push( 20 );
    rb.push( 30);
    rb.push( 40 );

    rb.peek().shallThrowWhenAssignedTo = true;

    rb.push( 50 ); //this does not overwrite, as we have S+1 capacity.
    ASSERT_THROW( rb.push( 60 ), int ); //This should cause 10 to be assigned to causing an exception.

    POP_AND_ASSERT_EQ(20);
    POP_AND_ASSERT_EQ(30);
    POP_AND_ASSERT_EQ(40);
    POP_AND_ASSERT_EQ(50);
    ASSERT_EQ(rb.size(), 0);

}


TEST(suiteName, test_set_integrity_maintained_when_T_assignment_operator_throws_exceptions_during_pop)
{
    Kjut::RingBuffer<ThrowingAssignmentOperator, 4> rb;
    rb.setOverflowPolicy(Kjut::OverflowPolicy::OverwriteOldestData);
    rb.push( 10 );
    rb.push( 20 );
    rb.push( 30 );
    rb.push( 40 );

    rb.peek().shallThrowWhenAssignedTo = true;

    int caught = 0;
    try
    {
        rb.pop(); //This should cause 10 to be deleted.
    }
    catch(int ten)
    {
       caught = ten;
    }
    ASSERT_EQ(caught, 10);
}


