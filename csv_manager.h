#ifndef CSV_MANAGER_H
#define CSV_MANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include "user.h"

FILE* open_file(const char* path, const char* mode);
int   read_all_users(const char* path, User** out);
bool  write_all_users_safe(const char* path, const User* list, int count);

bool add_user(const char* path, const User* in);
bool edit_user(const char* path, const char* key, const User* updated);
bool delete_user(const char* path, const char* key);
bool recycle_deleted_user(const User* u);
bool restore_user(const char* deleted_path, const char* users_path, const char* key);

void display_menu(void);
void display_table_header(void);
void display_user_row(const User* u);
bool validate_user(const User* u, char errbuf[], size_t errlen);
unsigned long long compute_checksum(const User* u);
int  generate_next_id(const char* path);
void update_auto_status(User* u);
void set_now(char out[NOW_LEN]);
void trim(char* s);
bool confirm_action(const char* message);

void run_builtin_unit_tests(const char* csv);

#endif
