#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../csv_manager.h"

static const char* TMP = "data/test_unit.csv";

static void reset(){
    FILE* f = fopen(TMP, "w");
    fprintf(f, "Name, Address, PhoneNumber\n");
    fprintf(f, "Unit A, Addr A, 100\n");
    fprintf(f, "Unit B, Addr B, 200\n");
    fclose(f);
}

void test_add_and_search(){
    reset();
    User u = { "New Guy", "Somewhere", "300" };
    assert(add_user(TMP, &u) && "add_user failed");

    User out[10];
    int n = search_user(TMP, "new", out, 10); // case-insensitive substring
    assert(n == 1);
    assert(strcmp(out[0].phone, "300")==0);
}

void test_edit_and_delete(){
    reset();
    User updated = { "Unit B2", "Addr B2", "201" };
    assert(edit_user(TMP, "200", &updated) && "edit failed");

    User out[10];
    int n = search_user(TMP, "201", out, 10);
    assert(n == 1 && strcmp(out[0].name, "Unit B2")==0);

    assert(delete_user(TMP, "Unit B2") && "delete failed");
    n = search_user(TMP, "201", out, 10);
    assert(n == 0);
}

int main(){
    test_add_and_search();
    test_edit_and_delete();
    printf("All unit tests passed.\n");
    return 0;
}
