#include <gtest/gtest.h>

#include <kjut/LoggingSystem.h>
#include <kjut/Application.h>

#define suiteName tst_logentry

static_assert( KJUT_LOGENTRY_MESSAGE_MAX_LENGTH == 15, "This test is designed for a KJUT_LOGENTRY_MESSAGE_MAX_LENGTH of 15");
static_assert( KJUT_LOGENTRY_FILENAME_MAX_LENGTH == 15, "This test is designed for a KJUT_LOGENTRY_FILENAME_MAX_LENGTH of 15");


TEST(suiteName, test_format_of_message)
{
    Kjut::LogEntry le;
    le.format("%d \"%s\"", 47, "FOO");
    ASSERT_STREQ(le.message(), "47 \"FOO\"");
}
TEST(suiteName, test_format_of_message_within_size)
{
    Kjut::LogEntry le;
    le.format("0123456789 :%d", 470);
    ASSERT_STREQ(le.message(), "0123456789 :470");
}

TEST(suiteName, test_format_of_message_just_larger_than_size)
{
    Kjut::LogEntry le;
    le.format("0123456789 :%d", 4700);
    ASSERT_STREQ(le.message(), "0123456789 :...");
}

TEST(suiteName, test_set_message_as_nullterminated_char_array_within_size)
{
    Kjut::LogEntry le;
    le.setMessage("0123456789abcde");
    ASSERT_STREQ(le.message(), "0123456789abcde");
}

TEST(suiteName, test_set_message_as_nullterminated_char_array_just_out_of_size)
{
    Kjut::LogEntry le;
    le.setMessage("0123456789abcdef");
    ASSERT_STREQ(le.message(), "0123456789ab...");
}


TEST(suiteName, test_set_filename_with_backslashes)
{
    Kjut::LogEntry le;
    le.setOriginatingFile("c:\\win\\dows\\path\\style\\bongo.cpp");
    ASSERT_STREQ(le.originatingFile(), "bongo.cpp");
}

TEST(suiteName, test_set_filename_with_forward_slashes)
{
    Kjut::LogEntry le;
    le.setOriginatingFile("/for/ward/slash/style/bongo.cpp");
    ASSERT_STREQ(le.originatingFile(), "bongo.cpp");
}

TEST(suiteName, test_set_filename_with_no_slashes_at_all)
{
    Kjut::LogEntry le;
    le.setOriginatingFile("bongo.cpp");
    ASSERT_STREQ(le.originatingFile(), "bongo.cpp");
}


TEST(suiteName, test_set_super_long_filename_with_no_slashes_at_all)
{
    Kjut::LogEntry le;
    le.setOriginatingFile("bongo-bongo-super-long.cpp");
    ASSERT_STREQ(le.originatingFile(), "bongo-...ng.cpp");
}


TEST(suiteName, test_set_super_long_filename_with_forward_slashes)
{
    Kjut::LogEntry le;
    le.setOriginatingFile("/for/ward/slashes/bongo-bongo-super-long.cpp");
    ASSERT_STREQ(le.originatingFile(), "bongo-...ng.cpp");
}

TEST(suiteName, test_set_super_long_filename_with_backward_slashes)
{
    Kjut::LogEntry le;
    le.setOriginatingFile("c:\\back\\ward\\slashes\\bongo-bongo-super-long.cpp");
    ASSERT_STREQ(le.originatingFile(), "bongo-...ng.cpp");
}

TEST(suiteName, test_send_to_application_instance)
{

    LOG("%s drums are %s!", "Bongo", "AWESOME");
}
