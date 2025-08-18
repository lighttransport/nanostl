#define NANOSTL_IMPLEMENTATION
#include "nanothread.h"
#include <iostream>
#include <cstdio>

using namespace nanostl;

// Simple function to run in a thread
void worker_function(int id, int iterations) {
    printf("Worker %d: Starting with %d iterations\n", id, iterations);
    
    for (int i = 0; i < iterations; i++) {
        // Simulate some work
        for (volatile int j = 0; j < 1000000; j++) {
            // Busy work
        }
        printf("Worker %d: Completed iteration %d\n", id, i + 1);
    }
    
    printf("Worker %d: Finished!\n", id);
}

int main() {
    printf("NanoSTL Thread Example\n");
    printf("======================\n\n");
    
    // Display hardware concurrency
    unsigned cores = thread::hardware_concurrency();
    printf("Hardware concurrency: %u cores\n\n", cores);
    
    // Get main thread ID
    thread::id main_id = this_thread::get_id();
    printf("Main thread running\n\n");
    
    // Create threads with lambda
    printf("Creating threads with lambdas...\n");
    
    thread t1([] {
        printf("Lambda thread: Hello from thread!\n");
    });
    
    // Create thread with function and arguments
    thread t2(worker_function, 1, 3);
    thread t3(worker_function, 2, 2);
    
    // Join all threads
    printf("Waiting for threads to complete...\n");
    
    if (t1.joinable()) {
        t1.join();
        printf("Lambda thread joined\n");
    }
    
    if (t2.joinable()) {
        t2.join();
        printf("Worker thread 1 joined\n");
    }
    
    if (t3.joinable()) {
        t3.join(); 
        printf("Worker thread 2 joined\n");
    }
    
    // Test thread move semantics
    printf("\nTesting thread move semantics...\n");
    
    thread t4([] {
        printf("Thread 4: Before move\n");
    });
    
    thread t5 = nanostl::move(t4);
    
    printf("Thread 4 joinable after move: %s\n", t4.joinable() ? "true" : "false");
    printf("Thread 5 joinable after move: %s\n", t5.joinable() ? "true" : "false");
    
    if (t5.joinable()) {
        t5.join();
        printf("Moved thread joined\n");
    }
    
    // Test this_thread functions
    printf("\nTesting this_thread functions...\n");
    printf("Yielding main thread...\n");
    this_thread::yield();
    
    printf("Sleeping for 50ms...\n");
    this_thread::sleep_for_ns(50000000); // 50 million nanoseconds = 50ms
    
    printf("\nExample completed successfully!\n");
    
    return 0;
}