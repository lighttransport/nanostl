#define NANOSTL_IMPLEMENTATION
#include "../include/nanotuple.h"
#include "../include/nanoutility.h"
#include "acutest.h"

using namespace nanostl;

// Test basic tuple creation and access
void test_basic_tuple() {
    tuple<int, double, char> t(42, 3.14, 'a');
    
    TEST_CHECK(get<0>(t) == 42);
    TEST_CHECK(get<1>(t) == 3.14);
    TEST_CHECK(get<2>(t) == 'a');
}

// Test tuple_size
void test_tuple_size() {
    using T1 = tuple<>;
    using T2 = tuple<int>;
    using T3 = tuple<int, double, char>;
    
    TEST_CHECK(tuple_size<T1>::value == 0);
    TEST_CHECK(tuple_size<T2>::value == 1);
    TEST_CHECK(tuple_size<T3>::value == 3);
}

// Test tuple_element
void test_tuple_element() {
    using T = tuple<int, double, char>;
    
    TEST_CHECK((is_same<tuple_element_t<0, T>, int>::value));
    TEST_CHECK((is_same<tuple_element_t<1, T>, double>::value));
    TEST_CHECK((is_same<tuple_element_t<2, T>, char>::value));
}

// Test make_tuple
void test_make_tuple() {
    auto t = make_tuple(42, 3.14, 'a');
    
    TEST_CHECK(get<0>(t) == 42);
    TEST_CHECK(get<1>(t) == 3.14);
    TEST_CHECK(get<2>(t) == 'a');
    
    // Test type deduction
    TEST_CHECK((is_same<decltype(t), tuple<int, double, char>>::value));
}

// Test tie
void test_tie() {
    int i = 0;
    double d = 0.0;
    char c = '\0';
    
    auto t = make_tuple(42, 3.14, 'a');
    tie(i, d, c) = t;
    
    TEST_CHECK(i == 42);
    TEST_CHECK(d == 3.14);
    TEST_CHECK(c == 'a');
}

// Test tie with ignore
void test_tie_ignore() {
    int i = 0;
    char c = '\0';
    
    auto t = make_tuple(42, 3.14, 'a');
    tie(i, ignore, c) = t;
    
    TEST_CHECK(i == 42);
    TEST_CHECK(c == 'a');
}

// Test forward_as_tuple
void test_forward_as_tuple() {
    int x = 42;
    double y = 3.14;
    
    auto t = forward_as_tuple(x, y);
    
    // Modify through the tuple
    get<0>(t) = 100;
    get<1>(t) = 2.71;
    
    TEST_CHECK(x == 100);
    TEST_CHECK(y == 2.71);
}

// Test tuple comparison operators
void test_tuple_comparison() {
    auto t1 = make_tuple(1, 2, 3);
    auto t2 = make_tuple(1, 2, 3);
    auto t3 = make_tuple(1, 2, 4);
    auto t4 = make_tuple(0, 2, 3);
    
    // Test equality
    TEST_CHECK(t1 == t2);
    TEST_CHECK(!(t1 == t3));
    TEST_CHECK(!(t1 == t4));
    
    // Test inequality
    TEST_CHECK(!(t1 != t2));
    TEST_CHECK(t1 != t3);
    TEST_CHECK(t1 != t4);
    
    // Test less than
    TEST_CHECK(!(t1 < t2));
    TEST_CHECK(t1 < t3);
    TEST_CHECK(!(t1 < t4));
    TEST_CHECK(t4 < t1);
    
    // Test other comparison operators
    TEST_CHECK(t1 <= t2);
    TEST_CHECK(t1 <= t3);
    TEST_CHECK(t1 >= t2);
    TEST_CHECK(!(t1 >= t3));
    TEST_CHECK(t3 > t1);
    TEST_CHECK(!(t1 > t3));
}

// Test tuple_cat
void test_tuple_cat() {
    auto t1 = make_tuple(1, 2);
    auto t2 = make_tuple(3.14, 'a');
    
    auto t3 = tuple_cat(t1, t2);
    
    TEST_CHECK(get<0>(t3) == 1);
    TEST_CHECK(get<1>(t3) == 2);
    TEST_CHECK(get<2>(t3) == 3.14);
    TEST_CHECK(get<3>(t3) == 'a');
    
    // Test type
    TEST_CHECK((is_same<decltype(t3), tuple<int, int, double, char>>::value));
}

// Test tuple assignment and copy
void test_tuple_assignment() {
    auto t1 = make_tuple(1, 2.0, 'a');
    auto t2 = make_tuple(3, 4.0, 'b');
    
    t1 = t2;
    
    TEST_CHECK(get<0>(t1) == 3);
    TEST_CHECK(get<1>(t1) == 4.0);
    TEST_CHECK(get<2>(t1) == 'b');
}

// Test empty tuple
void test_empty_tuple() {
    tuple<> t;
    
    TEST_CHECK(tuple_size<decltype(t)>::value == 0);
    
    // Test comparison with empty tuple
    tuple<> t2;
    TEST_CHECK(t == t2);
    TEST_CHECK(!(t < t2));
    TEST_CHECK(t <= t2);
    TEST_CHECK(t >= t2);
}

// Test single element tuple
void test_single_tuple() {
    tuple<int> t(42);
    
    TEST_CHECK(get<0>(t) == 42);
    TEST_CHECK(tuple_size<decltype(t)>::value == 1);
    
    auto t2 = make_tuple(42);
    TEST_CHECK(t == t2);
}

// Test tuple with references
void test_tuple_references() {
    int x = 42;
    double y = 3.14;
    
    tuple<int&, double&> t(x, y);
    
    get<0>(t) = 100;
    get<1>(t) = 2.71;
    
    TEST_CHECK(x == 100);
    TEST_CHECK(y == 2.71);
}

// Test const tuple operations
void test_const_tuple() {
    const auto t = make_tuple(42, 3.14, 'a');
    
    TEST_CHECK(get<0>(t) == 42);
    TEST_CHECK(get<1>(t) == 3.14);
    TEST_CHECK(get<2>(t) == 'a');
    
    // Test tuple_size with const
    TEST_CHECK(tuple_size<decltype(t)>::value == 3);
    
    // Test tuple_element with const
    TEST_CHECK((is_same<tuple_element_t<0, decltype(t)>, const int>::value));
}

// Test complex types in tuple (simplified without string/vector)
void test_complex_types() {
    tuple<int*, float> t;
    
    int x = 42;
    get<0>(t) = &x;
    get<1>(t) = 3.14f;
    
    TEST_CHECK(*get<0>(t) == 42);
    TEST_CHECK(get<1>(t) == 3.14f);
}

TEST_LIST = {
    {"basic_tuple", test_basic_tuple},
    {"tuple_size", test_tuple_size},
    {"tuple_element", test_tuple_element},
    {"make_tuple", test_make_tuple},
    {"tie", test_tie},
    {"tie_ignore", test_tie_ignore},
    {"forward_as_tuple", test_forward_as_tuple},
    {"tuple_comparison", test_tuple_comparison},
    {"tuple_cat", test_tuple_cat},
    {"tuple_assignment", test_tuple_assignment},
    {"empty_tuple", test_empty_tuple},
    {"single_tuple", test_single_tuple},
    {"tuple_references", test_tuple_references},
    {"const_tuple", test_const_tuple},
    {"complex_types", test_complex_types},
    {nullptr, nullptr}
};