#include <gtest/gtest.h>
#include <kjut/ByteArray.h>
#define suiteName tst_byte_array
TEST(suiteName, test_addition_and_removal_and_const_Data)
{
    Kjut::ByteArray ba;
    ba.append('q');
    ba.append('w');
    ba.append('e');
    ba.append('r');
    ba.append('t');
    ba.append('y');
    ba.append('u');
    ba.append('i');
    ba.append('o');
    ba.append('p');
    ba.append('a');
    ba.append('s');
    ba.append('d');
    ba.append('f');
    ba.append('g');
    ba.append('h');
    ba.append('i');
    ba.append('j');
    ba.append('k');
    ba.append('l');
    ba.append('\0');

    ASSERT_EQ(ba.size(), 21);
    ASSERT_STREQ(ba.constData(), "qwertyuiopasdfghijkl");
}

