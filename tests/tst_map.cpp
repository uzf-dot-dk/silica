#include <gtest/gtest.h>


#include <kjut/Map.h>

#define suiteName tst_map

std::ostream &operator<<(std::ostream &os,  Kjut::Map<std::string, int> & map) {
    os << "{";
    const Kjut::Array<std::string> &keys = map.keys();
    for(size_t i = 0; i < keys.size(); i++)
    {
        if(i > 0)
        {
            os << ", ";
        }
        os << "\"" << keys[i] << "\" => ";
        os << map[keys[i]];
    }
    os << "}";
    return os;
}

TEST(suiteName, test_insert_of_complex_K_type)
{
    Kjut::Map<std::string, int, 10> map;
    ASSERT_NE(117, map["foo"]);
    map.insert("foo", 117);
    ASSERT_EQ(117, map["foo"]);
}

TEST(suiteName, test_insert_of_complex_K_type_and_complex_V_type)
{
    Kjut::Map<std::string, std::string, 10> map;
    ASSERT_NE("Chewbacca", map["wookie"]);
    map.insert("wookie", "Chewbacca");
    ASSERT_EQ("Chewbacca", map["wookie"]);
}


TEST(suiteName,  test_range_based_for_loop_with_structured_bindings)
{
    Kjut::Map<std::string, int, 10> mapUnderTest;
    const std::map<std::string, int> inputMap =
        {
         {"foo" , 4 },
         {"bar", 27},
         {"foobar", 427},
         };

    std::map<std::string, int> outputMap;

    for (const auto & [key, value] : inputMap)
    {
        mapUnderTest.insert(key, value);
    }

    for (const auto & [key, value] : mapUnderTest)
    {
        outputMap[key] = value;
    }

    ASSERT_EQ(inputMap, outputMap);
}


TEST(suiteName,  test_range_based_for_loop_with_structured_bindings_on_empty_map)
{
    Kjut::Map<std::string, int, 10> mapUnderTest;
    const std::map<std::string, int> inputMap;
    std::map<std::string, int> outputMap;

    for (const auto & [key, value] : inputMap)
    {
        mapUnderTest.insert(key, value);
    }

    for (const auto & [key, value] : mapUnderTest)
    {
        outputMap[key] = value;
    }

    ASSERT_EQ(inputMap, outputMap);
}


TEST(suiteName,  test_erase_on_existing_keys)
{
    const std::map<std::string, int> input = {
        {"water", 11},
        {"tea", 55},
        {"coffee", 22}
    };

    const std::string keyToRemove = "tea";

    const std::map<std::string, int> expected = {
        {"water", 11},
        {"coffee", 22}
    };

    std::map<std::string, int> result;

    Kjut::Map<std::string, int> beverages;
    for (const auto & [key, value] : input)
    {
        beverages.insert(key, value);
    }

    beverages.erase(keyToRemove);
    for (const auto & [key, value] : beverages)
    {
        result[key] = value;
    }

    ASSERT_EQ(expected, result);

}
