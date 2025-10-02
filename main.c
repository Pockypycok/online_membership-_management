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
int choice;
    while(1){
        display_menu();
        if(scanf("%d", &choice)!=1){ printf("Invalid input\n"); return 0; }
        getchar(); // eat newline

        if(choice==0) break;

        if(choice==1){
            User u;
            printf("Name: ");    fgets(u.name, sizeof(u.name), stdin);       trim(u.name);
            printf("Address: "); fgets(u.address, sizeof(u.address), stdin);  trim(u.address);
            printf("Phone: ");   fgets(u.phone, sizeof(u.phone), stdin);      trim(u.phone);
            if(add_user(csv_path, &u)) printf("Added.\n"); else printf("Add failed.\n");

        }else if(choice==2){
            char key[64];
            printf("Search by name (substring) or phone (exact): ");
            fgets(key, sizeof(key), stdin); trim(key);
            User results[128];
            int n = search_user(csv_path, key, results, 128);
            printf("Found %d result(s)\n", n);
            for(int i=0;i<n && i<128;i++) print_user(&results[i]);

        }else if(choice==3){
            char key[64];
            printf("Edit target (name or phone): "); fgets(key, sizeof(key), stdin); trim(key);
            User u;
            printf("New name: ");    fgets(u.name, sizeof(u.name), stdin);       trim(u.name);
            printf("New address: "); fgets(u.address, sizeof(u.address), stdin);  trim(u.address);
            printf("New phone: ");   fgets(u.phone, sizeof(u.phone), stdin);      trim(u.phone);
            if(edit_user(csv_path, key, &u)) printf("Edited.\n"); else printf("Edit failed (not found?).\n");

        }else if(choice==4){
            char key[64];
            printf("Delete target (name or phone): ");
            fgets(key, sizeof(key), stdin); trim(key);
            if(delete_user(csv_path, key)) printf("Deleted.\n"); else printf("Delete failed (not found?).\n");

        }else if(choice==5){
            show_all(csv_path);

        }else{
            printf("Unknown choice.\n");
        }
    }
    return 0;
}
