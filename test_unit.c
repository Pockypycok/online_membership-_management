#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "csv_manager.h"

#define TEST_CSV "test_users.csv"

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

static int total = 0, passed = 0;

static void reset_file(const char* path) {
    FILE* f = fopen(path, "w");
    fprintf(f, "user_id,name,service,start_date,duration,expiry_date,status,email,phone,last_updated,checksum\n");
    fclose(f);
}

static void check(bool cond, const char* msg) {
    total++;
    if (cond) {
        printf(GREEN "PASS: %s\n" RESET, msg);
        passed++;
    } else {
        printf(RED "FAIL: %s\n" RESET, msg);
    }
}

/* --- Test Case 1: Add User --- */
static void test_add_user() {
    reset_file(TEST_CSV);
    User u = {0};
    strcpy(u.name, "Alice Wonderland");
    strcpy(u.service, "Premium");
    strcpy(u.start_date, "2025-10-09");
    strcpy(u.duration, "12M");
    strcpy(u.expiry_date, "2026-10-09");
    strcpy(u.status, "Active");
    strcpy(u.email, "alice@example.com");
    strcpy(u.phone, "0812345678");

    bool ok = add_user(TEST_CSV, &u);
    check(ok, "Add user (Alice)");

    User* arr = NULL;
    int n = read_all_users(TEST_CSV, &arr);
    check(n == 1, "File should contain 1 user after add");
    free(arr);
}

/* --- Test Case 2: Edit User --- */
static void test_edit_user() {
    User u = {0};
    strcpy(u.name, "Alice Updated");
    strcpy(u.service, "VIP");
    strcpy(u.start_date, "2025-10-10");
    strcpy(u.duration, "24M");
    strcpy(u.expiry_date, "2027-10-10");
    strcpy(u.status, "Active");
    strcpy(u.email, "alice_new@example.com");
    strcpy(u.phone, "0899999999");

    bool ok = edit_user(TEST_CSV, "Alice", &u);
    check(ok, "Edit user by name");

    User* arr = NULL;
    int n = read_all_users(TEST_CSV, &arr);
    check(strcmp(arr[0].service, "VIP") == 0, "User service updated to VIP");
    free(arr);
}

/* --- Test Case 3: Delete User --- */
static void test_delete_user() {
    bool ok = delete_user(TEST_CSV, "Alice");
    check(ok, "Delete user by name");

    User* arr = NULL;
    int n = read_all_users(TEST_CSV, &arr);
    check(n == 0, "File should be empty after delete");
    free(arr);
}

/* --- Test Case 4: Validation --- */
static void test_validation() {
    User u = {0};
    strcpy(u.name, "");
    strcpy(u.email, "bademail");
    strcpy(u.phone, "12");
    char err[100];
    bool ok = validate_user(&u, err, sizeof(err));
    check(!ok, "Invalid user validation failed correctly");
}

/* --- Test Case 5: Recycle Bin --- */
static void test_recycle_bin() {
    FILE* f = fopen("deleted_users.csv", "r");
    check(f != NULL, "Recycle bin file exists after delete");
    if (f) fclose(f);
}

/* --- Summary --- */
static void summary() {
    printf(CYAN "\n───────────────────────────────\n" RESET);
    printf("Total tests: %d\n", total);
    printf(GREEN "Passed: %d\n" RESET, passed);
    printf(RED   "Failed: %d\n" RESET, total - passed);
    printf(CYAN "───────────────────────────────\n" RESET);
    if (passed == total)
        printf(GREEN "ALL TESTS PASSED SUCCESSFULLY!\n" RESET);
    else
        printf(RED "Some tests failed. Check logic.\n" RESET);
}

/* --- Main --- */
int main() {
    printf(CYAN "\n RUNNING UNIT TESTS FOR MEMBERSHIP SYSTEM...\n" RESET);
    test_add_user();
    test_edit_user();
    test_delete_user();
    test_validation();
    test_recycle_bin();
    summary();
    return 0;
}
