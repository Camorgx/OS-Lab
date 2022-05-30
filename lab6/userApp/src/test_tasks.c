#include "test_tasks.h"
#include "userInterface.h"

void task1(void) {
    printf(0x7, "This is task 1 with priority 2 and arrive time 100.\n");
}

void task2(void) {
    printf(0x7, "This is task 2 with priority 6 and arrive time 100.\n");
}

void task3(void) {
    printf(0x7, "This is task 3 with priority 3 and arrive time 100.\n");
}

void task4(void) {
    printf(0x7, "This is task 4 with priority 5 and arrive time 100.\n");
}

void task5(void) {
    printf(0x7, "This is task 5 with priority 4 and arrive time 200.\n");
}