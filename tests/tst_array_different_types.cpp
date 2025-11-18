#include <gtest/gtest.h>
#include <silica/Array.h>

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

class String
{
public:
    String() {}
    String(const String &other)
    {
        copyConstructions.push_back({value, other.value});
        value = other.value;
    }
    String(String &&other)
    {
        moveConstructions.push_back({value, other.value});
        value = std::move(other.value);
    }

    ~String()
    {
        deletions.push_back(value);
    }

    String(const char* raw)
    {
        value = std::string(raw);
    }

    String & operator=(const String &other)
    {
        copyAssignments.push_back({value, other.value});
        value = other.value;
        return *this;
    }
    String & operator=(String &&other){
        moveAssignments.push_back({value, other.value});
        value = std::move(other.value);
        return *this;
    }

    bool operator==(const String &other) const
    {
        return value == other.value;
    }

    std::string value;

    static void clearAllRecords()
    {
        copyConstructions.clear();
        moveConstructions.clear();
        moveAssignments.clear();
        copyAssignments.clear();
        deletions.clear();
    }

    typedef struct participants
    {

        std::string destination;
        std::string source;
    } participants;

    static std::vector<participants> moveConstructions;
    static std::vector<participants> copyConstructions;
    static std::vector<participants> moveAssignments;
    static std::vector<participants> copyAssignments;
    static std::vector<std::string> deletions;

};

std::vector<String::participants> String::moveConstructions;
std::vector<String::participants> String::copyConstructions;
std::vector<String::participants> String::moveAssignments;
std::vector<String::participants> String::copyAssignments;
std::vector<std::string> String::deletions;

std::ostream& operator<<(std::ostream& os, const String& str)
{
    os << str.value;
    return os;
}

std::ostream& operator<<(std::ostream & os, Kjut::Array<String>& array)
{
    os << "[";
    std::string glue = "";
    for(auto &s: array)
    {
        os << glue << "\"" << s << "\"";
        glue = ", ";
    }
    os << "]";
    return os;
}

TEST(suiteName, test_insertion_of_strings_in_the_middle)
{
    Kjut::Array<String> array = {"Ant", "Bee", "Dog"};
    //array.insert(2, "Cat");
    std::cout << array << std::endl;


}
