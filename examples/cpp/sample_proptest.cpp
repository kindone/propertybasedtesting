#include "proptest/proptest.hpp"
#include "proptest/test/gtest.hpp"

#include <algorithm>
#include <numeric>
#include <cmath>

using namespace proptest;

// ---------------------------------------------------------------------------
// 1. Sorting: a sorted vector is always ordered and a permutation of input
// ---------------------------------------------------------------------------
TEST(SortProperty, SortedOutputIsOrdered)
{
    EXPECT_FOR_ALL([](vector<int> vec) {
        auto sorted = vec;
        std::sort(sorted.begin(), sorted.end());
        for (size_t i = 1; i < sorted.size(); i++)
            PROP_ASSERT_LE(sorted[i - 1], sorted[i]);
    });
}

TEST(SortProperty, SortPreservesLength)
{
    EXPECT_FOR_ALL([](vector<int> vec) {
        auto sorted = vec;
        std::sort(sorted.begin(), sorted.end());
        PROP_ASSERT(sorted.size() == vec.size());
    });
}

TEST(SortProperty, SortIsPermutation)
{
    EXPECT_FOR_ALL([](vector<int> vec) {
        auto sorted = vec;
        std::sort(sorted.begin(), sorted.end());
        PROP_ASSERT(std::is_permutation(sorted.begin(), sorted.end(), vec.begin()));
    });
}

TEST(SortProperty, SortIdempotent)
{
    EXPECT_FOR_ALL([](vector<int> vec) {
        auto once = vec;
        std::sort(once.begin(), once.end());
        auto twice = once;
        std::sort(twice.begin(), twice.end());
        PROP_ASSERT(once == twice);
    });
}

// ---------------------------------------------------------------------------
// 2. Reverse: reversing twice yields the original
// ---------------------------------------------------------------------------
TEST(ReverseProperty, DoubleReverseIsIdentity)
{
    EXPECT_FOR_ALL([](vector<int> vec) {
        auto copy = vec;
        std::reverse(copy.begin(), copy.end());
        std::reverse(copy.begin(), copy.end());
        PROP_ASSERT(copy == vec);
    });
}

TEST(ReverseProperty, ReversePreservesLength)
{
    EXPECT_FOR_ALL([](vector<int> vec) {
        auto rev = vec;
        std::reverse(rev.begin(), rev.end());
        PROP_ASSERT(rev.size() == vec.size());
    });
}

// ---------------------------------------------------------------------------
// 3. Integer arithmetic properties
// ---------------------------------------------------------------------------
TEST(ArithmeticProperty, AdditionCommutative)
{
    EXPECT_FOR_ALL([](int a, int b) {
        PROP_ASSERT_EQ(a + b, b + a);
    });
}

TEST(ArithmeticProperty, MultiplicationCommutative)
{
    EXPECT_FOR_ALL([](int a, int b) {
        PROP_ASSERT_EQ(a * b, b * a);
    });
}

TEST(ArithmeticProperty, AbsNonNegative)
{
    auto smallInt = gen::interval<int>(-1000000, 1000000);
    EXPECT_FOR_ALL([](int x) {
        PROP_ASSERT_GE(std::abs(x), 0);
    }, smallInt);
}

// ---------------------------------------------------------------------------
// 4. String properties
// ---------------------------------------------------------------------------
TEST(StringProperty, ConcatLength)
{
    EXPECT_FOR_ALL([](string a, string b) {
        PROP_ASSERT_EQ((a + b).size(), a.size() + b.size());
    });
}

TEST(StringProperty, SubstrRoundtrip)
{
    auto strGen = gen::string();
    EXPECT_FOR_ALL([](string s) {
        if (!s.empty()) {
            auto prefix = s.substr(0, s.size() / 2);
            auto suffix = s.substr(s.size() / 2);
            PROP_ASSERT_EQ(prefix + suffix, s);
        }
    }, strGen);
}

// ---------------------------------------------------------------------------
// 5. Custom generator: even numbers stay even after arithmetic
// ---------------------------------------------------------------------------
TEST(CustomGenProperty, EvenTimesAnyIsEven)
{
    auto evenGen = gen::interval<int>(-10000, 10000).map([](int n) {
        return n * 2;
    });

    EXPECT_FOR_ALL([](int even, int any) {
        PROP_ASSERT_EQ(even * any % 2, 0);
    }, evenGen, gen::interval<int>(-10000, 10000));
}

// ---------------------------------------------------------------------------
// 6. Map (std::map) insertion property
// ---------------------------------------------------------------------------
TEST(MapProperty, InsertIncrementsSize)
{
    auto keyGen = gen::interval<int>(0, 1000);
    auto valGen = gen::interval<int>(0, 1000);

    EXPECT_FOR_ALL([](int key, int val) {
        std::map<int, int> m;
        m[key] = val;
        PROP_ASSERT_EQ(m.size(), static_cast<size_t>(1));
        PROP_ASSERT_EQ(m[key], val);
    }, keyGen, valGen);
}

// ---------------------------------------------------------------------------
// 7. Property API: using property() with .forAll() and .example()
// ---------------------------------------------------------------------------
TEST(PropertyAPI, SortWithExplicitPropertyObject)
{
    auto prop = property([](vector<int> vec) {
        auto sorted = vec;
        std::sort(sorted.begin(), sorted.end());
        PROP_ASSERT(std::is_sorted(sorted.begin(), sorted.end()));
    });

    prop.forAll();
    prop.example(vector<int>{});
    prop.example(vector<int>{42});
    prop.example(vector<int>{3, 1, 4, 1, 5, 9, 2, 6});
}
