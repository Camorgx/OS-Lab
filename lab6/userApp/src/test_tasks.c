#include "test_tasks.h"
#include "userInterface.h"

void task1(void) {
    printf(0x7, "This is task 1 with priority 2 and arrive time 100.\n");
    for (int i = 1; i <= 1000000000; ++i)
        if (i % 200000000 == 0) {
            printf(0x7, "task1 counter: %d\n", i / 200000000);
        }
}

void task2(void) {
    printf(0x7, "This is task 2 with priority 6 and arrive time 100.\n");
    for (int i = 1; i <= 1000000000; ++i)
        if (i % 200000000 == 0) {
            printf(0x7, "task2 counter: %d\n", i / 200000000);
        }
}

void task3(void) {
    printf(0x7, "This is task 3 with priority 3 and arrive time 100.\n");
    for (int i = 1; i <= 1000000000; ++i)
        if (i % 200000000 == 0) {
            printf(0x7, "task3 counter: %d\n", i / 200000000);
        }
}

void task4(void) {
    printf(0x7, "This is task 4 with priority 5 and arrive time 100.\n");
    for (int i = 1; i <= 1000000000; ++i)
        if (i % 200000000 == 0) {
            printf(0x7, "task4 counter: %d\n", i / 200000000);
        }
}

void task5(void) {
    printf(0x7, "This is task 5 with priority 4 and arrive time 200.\n");
    for (int i = 1; i <= 1000000000; ++i)
        if (i % 200000000 == 0) {
            printf(0x7, "task5 counter: %d\n", i / 200000000);
        }
}