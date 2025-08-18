#define NANOSTL_IMPLEMENTATION
#include "nanotuple.h"
#include "nanoutility.h"
#include <cstdio>

using namespace nanostl;

// Function that returns multiple values using tuple
tuple<int, double, bool> calculate_stats(int x) {
    return make_tuple(x * 2, x * 3.14, x > 10);
}

// Function that takes a tuple parameter
void print_tuple_info(const tuple<int, double, char>& t) {
    printf("Tuple contents: %d, %f, %c\n", get<0>(t), get<1>(t), get<2>(t));
    printf("Tuple size: %llu\n", (unsigned long long)tuple_size<decltype(t)>::value);
}

int main() {
    printf("NanoSTL Tuple Example\n");
    printf("====================\n\n");
    
    // 1. Basic tuple creation and access
    printf("1. Basic tuple operations:\n");
    tuple<int, double, char> basic_tuple(42, 3.14, 'A');
    printf("Created tuple: (%d, %f, %c)\n", get<0>(basic_tuple), get<1>(basic_tuple), get<2>(basic_tuple));
    
    // 2. make_tuple
    printf("\n2. Using make_tuple:\n");
    auto auto_tuple = make_tuple(100, 2.71, 'Z');
    printf("Auto tuple: (%d, %f, %c)\n", get<0>(auto_tuple), get<1>(auto_tuple), get<2>(auto_tuple));
    
    // 3. Tuple size and element type queries
    printf("\n3. Tuple traits:\n");
    printf("Tuple size: %llu\n", (unsigned long long)tuple_size<decltype(basic_tuple)>::value);
    printf("First element is int: %s\n", 
           is_same<tuple_element_t<0, decltype(basic_tuple)>, int>::value ? "true" : "false");
    printf("Second element is double: %s\n", 
           is_same<tuple_element_t<1, decltype(basic_tuple)>, double>::value ? "true" : "false");
    
    // 4. tie - unpacking tuples
    printf("\n4. Using tie to unpack:\n");
    int i;
    double d;
    char c;
    tie(i, d, c) = basic_tuple;
    printf("Unpacked values: i=%d, d=%f, c=%c\n", i, d, c);
    
    // 5. tie with ignore
    printf("\n5. Using tie with ignore:\n");
    int first, third;
    tie(first, ignore, third) = make_tuple(10, 20, 30);
    printf("First: %d, Third: %d (middle ignored)\n", first, third);
    
    // 6. forward_as_tuple - references
    printf("\n6. Forward as tuple (references):\n");
    int x = 5, y = 10;
    auto ref_tuple = forward_as_tuple(x, y);
    get<0>(ref_tuple) = 50;
    get<1>(ref_tuple) = 100;
    printf("Modified through tuple: x=%d, y=%d\n", x, y);
    
    // 7. Tuple comparison
    printf("\n7. Tuple comparison:\n");
    auto t1 = make_tuple(1, 2, 3);
    auto t2 = make_tuple(1, 2, 3);
    auto t3 = make_tuple(1, 2, 4);
    
    printf("t1 == t2: %s\n", (t1 == t2) ? "true" : "false");
    printf("t1 == t3: %s\n", (t1 == t3) ? "true" : "false");
    printf("t1 < t3: %s\n", (t1 < t3) ? "true" : "false");
    
    // 8. tuple_cat
    printf("\n8. Tuple concatenation:\n");
    auto part1 = make_tuple(1, 2);
    auto part2 = make_tuple(3.14, 'X');
    auto combined = tuple_cat(part1, part2);
    printf("Combined tuple: (%d, %d, %f, %c)\n", 
           get<0>(combined), get<1>(combined), get<2>(combined), get<3>(combined));
    
    // 9. Function returning tuple
    printf("\n9. Function returning multiple values:\n");
    auto stats = calculate_stats(15);
    printf("Stats for 15: doubled=%d, pi_mult=%f, greater_than_10=%s\n",
           get<0>(stats), get<1>(stats), get<2>(stats) ? "true" : "false");
    
    // 10. Empty tuple
    printf("\n10. Empty tuple:\n");
    tuple<> empty;
    printf("Empty tuple size: %llu\n", (unsigned long long)tuple_size<decltype(empty)>::value);
    
    // 11. Single element tuple
    printf("\n11. Single element tuple:\n");
    tuple<int> single(42);
    printf("Single tuple: (%d)\n", get<0>(single));
    
    // 12. Tuple with references
    printf("\n12. Tuple with references:\n");
    int ref_x = 100;
    double ref_y = 200.5;
    tuple<int&, double&> ref_tuple_direct(ref_x, ref_y);
    printf("Before modification: ref_x=%d, ref_y=%f\n", ref_x, ref_y);
    
    get<0>(ref_tuple_direct) = 999;
    get<1>(ref_tuple_direct) = 888.8;
    printf("After modification through tuple: ref_x=%d, ref_y=%f\n", ref_x, ref_y);
    
    // 13. Complex nested scenario
    printf("\n13. Nested tuple operations:\n");
    auto nested_data = make_tuple(
        make_tuple(1, 2),
        make_tuple(3.14, 2.71),
        'N'
    );
    
    printf("Nested access: first_tuple_first=%d, second_tuple_second=%f, char=%c\n",
           get<0>(get<0>(nested_data)),
           get<1>(get<1>(nested_data)),
           get<2>(nested_data));
    
    printf("\nTuple example completed successfully!\n");
    
    return 0;
}