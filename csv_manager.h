#ifndef CSV_MANAGER_H
#define CSV_MANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include "user.h"

#define NAME_LEN 96
#define SRV_LEN 96
#define DATE_LEN 32
#define DUR_LEN 32

typedef struct {
    char name[NAME_LEN];        // ชื่อผู้สมัคร
    char service[SRV_LEN];      // บริการที่สมัคร
    char start_date[DATE_LEN];  // วันที่สมัคร
    char duration[DUR_LEN];     // ระยะเวลาสมัคร
} Subscriber;

FILE* open_file(const char* path, const char* mode);
bool add_user(const char* path, const Subscriber* s);
bool edit_user(const char* path, const char* key, const Subscriber* updated);
bool delete_user(const char* path, const char* key);
int  search_user(const char* path, const char* key, Subscriber* out, int max_out);
int  read_all_users(const char* path, Subscriber** out_list);
bool write_all_users(const char* path, const Subscriber* list, int count);
void trim(char* s);
void display_menu(void);

#endif
