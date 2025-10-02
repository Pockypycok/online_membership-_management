#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdbool.h>

#define NAME_LEN 96
#define SRV_LEN 96
#define DATE_LEN 32
#define DUR_LEN 32

// โครงสร้างข้อมูลผู้สมัคร
typedef struct {
    char name[NAME_LEN];        // ชื่อผู้สมัคร
    char service[SRV_LEN];      // บริการที่สมัคร
    char start_date[DATE_LEN];  // วันที่สมัคร
    char duration[DUR_LEN];     // ระยะเวลาสมัคร
} User;

// ฟังก์ชันหลัก
FILE* open_file(const char* path, const char* mode);

// CRUD
bool add_user(const char* path, const User* u);
bool edit_user(const char* path, const char* key, const User* updated);
bool delete_user(const char* path, const char* key);
int  search_user(const char* path, const char* key, User* out, int max_out);

// Utility
int  read_all_users(const char* path, User** out_list);
bool write_all_users(const char* path, const User* list, int count);
void trim(char* s);
void display_menu(void);

#endif
