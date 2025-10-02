#ifndef CSV_MANAGER_H
#define CSV_MANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include "user.h"

// Open CSV file with mode ("r","w","a","r+")
FILE* open_file(const char* path, const char* mode);

// CRUD
bool add_user(const char* path, const User* user);
bool edit_user(const char* path, const char* key, const User* updated); // key = name or phone
bool delete_user(const char* path, const char* key);                    // key = name or phone
int  search_user(const char* path, const char* key, User* out_list, int max_out);

// Utilities
void display_menu(void);

// Helpers (ใช้ใน unit test)
int  read_all_users(const char* path, User** out_list); // malloc ให้ (*out_list)
bool write_all_users(const char* path, const User* list, int count);
void trim(char* s);

#endif
