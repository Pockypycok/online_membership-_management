#ifndef USER_H
#define USER_H

#define NAME_LEN   100
#define SRV_LEN    100
#define DATE_LEN   16
#define DUR_LEN    32
#define STATUS_LEN 16
#define EMAIL_LEN  128
#define PHONE_LEN  32
#define NOW_LEN    20

typedef struct {
    int  user_id;
    char name[NAME_LEN];
    char service[SRV_LEN];
    char start_date[DATE_LEN];
    char duration[DUR_LEN];
    char expiry_date[DATE_LEN];
    char status[STATUS_LEN];
    char email[EMAIL_LEN];
    char phone[PHONE_LEN];
    char last_updated[NOW_LEN];
    unsigned long long checksum;
} User;

#endif
