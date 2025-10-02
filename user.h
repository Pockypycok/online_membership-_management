#ifndef USER_H
#define USER_H

#define NAME_LEN 64
#define ADDR_LEN 128
#define PHONE_LEN 32

typedef struct {
    char name[NAME_LEN];
    char address[ADDR_LEN];
    char phone[PHONE_LEN];
} User;

#endif
