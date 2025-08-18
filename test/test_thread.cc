#define NANOSTL_IMPLEMENTATION
#include "../include/nanothread.h"
#include "../include/nanochrono.h"
#include "acutest.h"

using namespace nanostl;

// Test basic thread creation and join
void test_basic_thread() {
    static bool executed = false;
    
    thread t([] {
        executed = true;
    });
    
    TEST_CHECK(t.joinable());
    t.join();
    TEST_CHECK(!t.joinable());
    TEST_CHECK(executed);
}

// Test thread with arguments
void test_thread_with_args() {
    static int result = 0;
    
    auto func = [](int a, int b) {
        result = a + b;
    };
    
    thread t(func, 5, 10);
    t.join();
    
    TEST_CHECK(result == 15);
}

// Simple function for thread testing
static void simple_thread_func() {
    // Do some work
    for (int i = 0; i < 1000; i++) {
        // Busy work
    }
}

// Test thread with simple function
void test_simple_thread() {
    thread t(simple_thread_func);
    
    TEST_CHECK(t.joinable());
    t.join();
    TEST_CHECK(!t.joinable());
}

// Test hardware_concurrency
void test_hardware_concurrency() {
    unsigned int cores = thread::hardware_concurrency();
    TEST_CHECK(cores >= 1);
}

// Test this_thread::get_id
void test_thread_id() {
    thread::id main_id = this_thread::get_id();
    thread::id default_id;
    
    TEST_CHECK(main_id != default_id);
    TEST_CHECK(main_id == main_id);
}

// Test this_thread yield
void test_this_thread_yield() {
    // Test yield (hard to test meaningfully, just ensure it doesn't crash)
    this_thread::yield();
    TEST_CHECK(1); // If we get here, yield didn't crash
}

TEST_LIST = {
    {"basic_thread", test_basic_thread},
    {"thread_with_args", test_thread_with_args},
    {"simple_thread", test_simple_thread},
    {"hardware_concurrency", test_hardware_concurrency},
    {"thread_id", test_thread_id},
    {"this_thread_yield", test_this_thread_yield},
    {nullptr, nullptr}
};