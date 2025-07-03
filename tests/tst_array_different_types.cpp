#include <gtest/gtest.h>
#include <kjut/Array.h>

#define suiteName tst_array_different_types


TEST(suiteName, test_placement_new)
{
    Kjut::Array<std::string> countsDynamic;
    ASSERT_TRUE( countsDynamic.append("foo"));
    ASSERT_TRUE( countsDynamic.append("bar"));
    ASSERT_TRUE( countsDynamic.append("fubar"));

    ASSERT_EQ( countsDynamic.size(), 3);

    ASSERT_EQ(std::string("foo"), countsDynamic[0]);
    ASSERT_EQ(std::string("bar"), countsDynamic[1]);
    ASSERT_EQ(std::string("fubar"), countsDynamic[2]);
}


TEST(suiteName, test_erase_new)
{
    Kjut::Array<std::string> a;
    a.append("foo");
    a.append("bar");
    a.append("fubar");

    a.remove(1);

    ASSERT_EQ(std::string("foo"), a[0]);
    ASSERT_EQ(std::string("fubar"), a[1]);
}

