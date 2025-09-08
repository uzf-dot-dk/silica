#include <gtest/gtest.h>
#include <kjut/ByteArray.h>
#include <kjut/ScopeGuard.h>

#define suiteName tst_byte_array

using namespace Kjut;

class MyLogSink : public LogSink
{
public:

    MyLogSink()
    {
        resetCounts();
    }

    void resetCounts()
    {
        warnings = 0;
        logs = 0;
        fatals = 0;
    }
    void sinkEntry(const LogEntry &entry)
    {
        switch(entry.type())
        {
        case LogEntry::Type::Log:
            logs++;
            break;
        case LogEntry::Type::Warning:
            warnings++;
            break;
        case LogEntry::Type::Fatal:
            fatals++;
            break;
        }
    }

    size_t warnings;
    size_t logs;
    size_t fatals;
};




TEST(suiteName, test_appending_to_S_0_growing_array)
{
    ByteArray<> ba;
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
    const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
    ASSERT_STREQ(stringValue, "qwertyuiopasdfghijkl");
}



TEST(suiteName, test_appending_to_S_6_non_growing_array)
{
    /* Tag: WARNING_COUNT_TEST
     * Do not remove until you havemoved this warning counting to tst_array.
     */
    MyLogSink sinkInstance;
    LoggingSystem::instance()->setSink(&sinkInstance);

    ScopeGuard onExit(+[]()
    {
        LoggingSystem::instance()->setSink(nullptr);
    });

    ByteArray<6> ba;
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

    ASSERT_EQ(ba.size(), 6);
    const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
    const int comparisonResult = strncmp(stringValue, "qwerty", 6);
    ASSERT_EQ(comparisonResult, 0);
    ASSERT_EQ(sinkInstance.warnings, 15);
}


#define D(data) reinterpret_cast<const unsigned char*>(data), strlen(data)

TEST(suiteName, test_set_data_fixed_size)
{
    ByteArray<10> ba;

    const size_t bytes_stored = ba.setData(D("qwertyuiop_asdfghjkl;"));

    ASSERT_EQ(bytes_stored, 10);
    const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
    const int comparisonResult = strncmp(stringValue, "qwertyuiop", 10);
    ASSERT_EQ(comparisonResult, 0);

}



TEST(suiteName, test_set_data_growing_size_and_set_again)
{
    ByteArray<> ba;

    {
        const size_t bytes_stored = ba.setData(D("qwertyuiop_asdfghjkl"));

        ASSERT_EQ(bytes_stored, 20);
        const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
        const int comparisonResult = strncmp(stringValue, "qwertyuiop_asdfghjkl", 20);
        ASSERT_EQ(comparisonResult, 0);
    }
    {
        const size_t bytes_stored = ba.setData(D("QWERTYUIOP0123456789"));

        ASSERT_EQ(bytes_stored, 20);
        const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
        const int comparisonResult = strncmp(stringValue, "QWERTYUIOP0123456789", 20);
        ASSERT_EQ(comparisonResult, 0);
    }
}




TEST(suiteName, test_set_data_fixed_size_and_set_again)
{
    ByteArray<10> ba;

    {
        const size_t bytes_stored = ba.setData(D("qwertyuiop_asdfghjkl;"));

        ASSERT_EQ(bytes_stored, 10);
        const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
        const int comparisonResult = strncmp(stringValue, "qwertyuiop", 10);
        ASSERT_EQ(comparisonResult, 0);
    }

    {
        const size_t bytes_stored = ba.setData(D("QWERTYUIOP_asdfghjkl;"));

        ASSERT_EQ(bytes_stored, 10);
        const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
        const int comparisonResult = strncmp(stringValue, "QWERTYUIOP", 10);
        ASSERT_EQ(comparisonResult, 0);
    }
}


TEST(suiteName, test_append_to_empty_fixed_size)
{
    constexpr size_t S = 10;
    ByteArray<S> ba;

    const size_t appended = ba.appendData(D("asdfasdfzxcvtg"));
    ASSERT_EQ(appended, S);

    const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
    const int comparisonResult = strncmp(stringValue, "asdfasdfzx", S);
    ASSERT_EQ(comparisonResult, 0);
}


TEST(suiteName, test_append_to_empty_dynamic_size)
{
    constexpr size_t S = 14;
    ByteArray<> ba;

    const size_t appended = ba.appendData(D("asdfasdfzxcvtg"));
    ASSERT_EQ(appended, S);

    const char * stringValue =  reinterpret_cast<const char *>(ba.constData()) ;
    const int comparisonResult = strncmp(stringValue, "asdfasdfzxcvtg", S);
    ASSERT_EQ(comparisonResult, 0);
}




template <size_t S>
void storeSize(size_t *destination, ByteArray<S> &ba)
{
    (*destination) = ba.size();
};


TEST(suiteName, test_substitution_of_implicit_0_explicit_0_and_explicit_S)
{
    ByteArray<10> fixedTenLong;
    ByteArray<0> explicitlyGrowing;
    ByteArray<> implicitlyGrowing;

    for(size_t i = 0 ; i < 10; i++)
    {
        unsigned char value = 0x30 + i;
        fixedTenLong.append(value);
        explicitlyGrowing.append(value);
        implicitlyGrowing.append(value);
    }

    size_t size;

    storeSize(&size, fixedTenLong);
    ASSERT_EQ(size, 10);

    storeSize(&size, explicitlyGrowing);
    ASSERT_EQ(size, 10);

    storeSize(&size, implicitlyGrowing);
    ASSERT_EQ(size, 10);
}
