#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "csv_manager.h"

static void print_user(const User* u){
    printf("- %s | %s | %s\n", u->name, u->address, u->phone);
}

static void show_all(const char* path){
    User* arr=NULL; int n = read_all_users(path, &arr);
    if(n<=0){ printf("(no data)\n"); free(arr); return; }
    for(int i=0;i<n;i++) print_user(&arr[i]);
    free(arr);
}

int main(int argc, char** argv){
    const char* csv_path = (argc > 1) ? argv[1] : "data/users.csv";
    system("mkdir -p data");
