#include <gtest/gtest.h>
#include <kjut/SignalSlot.h>

#define suiteName tst_signal_and_slots

using namespace Kjut;


int test_simple_connection_free_floating_marker;
void test_simple_connection_free_floating(int i)
{
    test_simple_connection_free_floating_marker = i;
}
TEST(suiteName, test_simple_connection)
{
    Slot<int> watcher(test_simple_connection_free_floating);
    Signal<int> valueChanged;

    valueChanged.connectTo(&watcher);
    test_simple_connection_free_floating_marker = 0;
    ASSERT_EQ(test_simple_connection_free_floating_marker, 0);
    valueChanged(10);
    ASSERT_EQ(test_simple_connection_free_floating_marker, 10);
}



int test_dual_connection_free_floating_marker_one;
void test_dual_connection_free_floating_one(int i)
{
    test_dual_connection_free_floating_marker_one = i;
}
int test_dual_connection_free_floating_marker_two;
void test_dual_connection_free_floating_two(int i)
{
    test_dual_connection_free_floating_marker_two = i * 2;
}
TEST(suiteName, test_dual_connection)
{

    Slot<int> watcher_1(test_dual_connection_free_floating_one);
    Slot<int> watcher_2(test_dual_connection_free_floating_two);
    Signal<int> valueChanged;

    valueChanged.connectTo(&watcher_1);
    valueChanged.connectTo(&watcher_2);
    test_dual_connection_free_floating_marker_one = 0;
    test_dual_connection_free_floating_marker_two = 0;
    ASSERT_EQ(test_dual_connection_free_floating_marker_one, 0);
    ASSERT_EQ(test_dual_connection_free_floating_marker_two, 0);
    valueChanged(10);
    ASSERT_EQ(test_dual_connection_free_floating_marker_one, 10);
    ASSERT_EQ(test_dual_connection_free_floating_marker_two, 20);
}










int test_signal_to_signal_to_signal_to_slot_connection_value;
void test_signal_to_signal_to_signal_to_slot_connection_handler(int i)
{
    test_signal_to_signal_to_signal_to_slot_connection_value = i;
}

TEST(suiteName, test_signal_to_signal_to_slot_connection)
{

    Signal<int> valueChanged;
    Signal<int> relay;
    Slot<int> watcher(test_signal_to_signal_to_signal_to_slot_connection_handler);

    test_signal_to_signal_to_signal_to_slot_connection_value = 0;

    valueChanged.connectTo(&relay);
    relay.connectTo(&watcher);
    valueChanged(10);

    ASSERT_EQ(test_signal_to_signal_to_signal_to_slot_connection_value, 10);
}




class MySlotOwner
{

public:

    MySlotOwner()
        : printNewInt(std::bind(&MySlotOwner::doPrintNextImpl, this, std::placeholders::_1))
    {
    }

    void doPrintNextImpl(int i)
    {
        caught = i;
    }

    Slot<int> printNewInt;
    int caught = 0;
};


template <typename T, typename... Ts>
std::function<void(Ts...)> bind_method(T& obj, void (T::*method)(Ts...));

TEST(suiteName, test_signal_to_slot_to_class_instance)
{
    Signal<int> emitNewInt;
    MySlotOwner mso;
    emitNewInt.connectTo(&mso.printNewInt);

    ASSERT_EQ(mso.caught, 0);
    emitNewInt(117);
    ASSERT_EQ(mso.caught, 117);
}






std::string test_slot_deletions_doesnt_crash_slot_destination_a;
std::string test_slot_deletions_doesnt_crash_slot_destination_b;
std::string test_slot_deletions_doesnt_crash_slot_destination_c;
void test_slot_deletions_doesnt_crash_a(const std::string &str)
{
    test_slot_deletions_doesnt_crash_slot_destination_a = "A" + str;
}
void test_slot_deletions_doesnt_crash_b(const std::string &str)
{
    test_slot_deletions_doesnt_crash_slot_destination_b = "B" + str;
}
void test_slot_deletions_doesnt_crash_c(const std::string &str)
{
    test_slot_deletions_doesnt_crash_slot_destination_c = "C" + str;
}
TEST( suiteName, test_slot_deletions_doesnt_crash)
{
    Signal<std::string> source;
    Slot<std::string> slotA(test_slot_deletions_doesnt_crash_a);
    Slot<std::string> *slotB = new Slot<std::string>(test_slot_deletions_doesnt_crash_b);
    Slot<std::string> slotC(test_slot_deletions_doesnt_crash_c);

    source.connectTo( & slotA );
    source.connectTo(   slotB );
    source.connectTo( & slotC );

    {
        test_slot_deletions_doesnt_crash_slot_destination_a.clear();
        test_slot_deletions_doesnt_crash_slot_destination_b.clear();
        test_slot_deletions_doesnt_crash_slot_destination_c.clear();
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_a, std::string(""));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_b, std::string(""));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_c, std::string(""));
        emit source("FOO");
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_a, std::string("AFOO"));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_b, std::string("BFOO"));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_c, std::string("CFOO"));
    }

    delete slotB;

    {
        test_slot_deletions_doesnt_crash_slot_destination_a.clear();
        test_slot_deletions_doesnt_crash_slot_destination_b.clear();
        test_slot_deletions_doesnt_crash_slot_destination_c.clear();

        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_a, std::string(""));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_b, std::string(""));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_c, std::string(""));

        emit source("FOO");
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_a, std::string("AFOO"));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_b, std::string(""));
        ASSERT_EQ(test_slot_deletions_doesnt_crash_slot_destination_c, std::string("CFOO"));
    }
}
