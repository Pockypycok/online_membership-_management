#include "csv_manager.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void chomp(char* s){
    if(!s) return;
    size_t n=strlen(s);
    while(n>0 && (s[n-1]=='\n'||s[n-1]=='\r')) s[--n]='\0';
}

void trim(char* s){
    if(!s) return;
    size_t i=0; while(isspace((unsigned char)s[i])) i++;
    if(i) memmove(s, s+i, strlen(s+i)+1);
    size_t n=strlen(s);
    while(n>0 && isspace((unsigned char)s[n-1])) s[--n]='\0';
}

FILE* open_file(const char* path, const char* mode){
    return fopen(path, mode);
}

static bool parse_line(const char* line, User* u){
    // Format: Name, Address, PhoneNumber
    char n[NAME_LEN], a[ADDR_LEN], p[PHONE_LEN];
    if(sscanf(line, " %63[^,] , %127[^,] , %31[^\n\r]", n, a, p) == 3){
        strncpy(u->name, n, NAME_LEN);   u->name[NAME_LEN-1]='\0';   trim(u->name);
        strncpy(u->address, a, ADDR_LEN);u->address[ADDR_LEN-1]='\0';trim(u->address);
        strncpy(u->phone, p, PHONE_LEN); u->phone[PHONE_LEN-1]='\0'; trim(u->phone);
        return true;
    }
    return false;
}

static void format_line(char* buf, size_t buflen, const User* u){
    snprintf(buf, buflen, "%s, %s, %s\n", u->name, u->address, u->phone);
}

int read_all_users(const char* path, User** out_list){
    *out_list = NULL;
    FILE* f = open_file(path, "r");
    if(!f) return 0;

    char line[512];
    int cap=16, cnt=0;
    User* arr = (User*)malloc(sizeof(User)*cap);
    if(!arr){ fclose(f); return 0; }

    while(fgets(line, sizeof(line), f)){
        chomp(line);
        if(line[0]=='\0') continue;
        if(strncasecmp(line, "Name,", 5)==0) continue; // skip header
        User u;
        if(parse_line(line, &u)){
            if(cnt>=cap){
                cap*=2;
                arr = (User*)realloc(arr, sizeof(User)*cap);
                if(!arr){ fclose(f); return 0; }
            }
            arr[cnt++] = u;
        }
    }
    fclose(f);
    *out_list = arr;
    return cnt;
}

bool write_all_users(const char* path, const User* list, int count){
    FILE* f = open_file(path, "w");
    if(!f) return false;
    fprintf(f, "Name, Address, PhoneNumber\n");
    for(int i=0;i<count;i++){
        char buf[512];
        format_line(buf, sizeof(buf), &list[i]);
        fputs(buf, f);
    }
    fclose(f);
    return true;
}

bool add_user(const char* path, const User* user){
    FILE* f = fopen(path, "r");
    if(!f){
        FILE* fw = fopen(path, "w");
        if(!fw) return false;
        fprintf(fw, "Name, Address, PhoneNumber\n");
        fclose(fw);
    }else fclose(f);

    FILE* fa = open_file(path, "a");
    if(!fa) return false;
    char buf[512];
    format_line(buf, sizeof(buf), user);
    fputs(buf, fa);
    fclose(fa);
    return true;
}

static int match_key(const User* u, const char* key){
    // ชื่อ (ไม่สนเคส) หรือ เบอร์ (ตรงเป๊ะ)
    char name_lower[NAME_LEN], key_lower[NAME_LEN];
    strncpy(name_lower, u->name, NAME_LEN); name_lower[NAME_LEN-1]='\0';
    strncpy(key_lower, key, NAME_LEN);      key_lower[NAME_LEN-1]='\0';
    for(char* c=name_lower; *c; ++c) *c = tolower((unsigned char)*c);
    for(char* c=key_lower; *c; ++c) *c = tolower((unsigned char)*c);
    if(strcmp(name_lower, key_lower)==0) return 1;
    if(strcmp(u->phone, key)==0)          return 1;
    return 0;
}

bool edit_user(const char* path, const char* key, const User* updated){
    User* arr=NULL; int n = read_all_users(path, &arr);
    if(n<=0){ free(arr); return false; }
    int changed=0;
    for(int i=0;i<n;i++){
        if(match_key(&arr[i], key)){ arr[i] = *updated; changed=1; break; }
    }
    bool ok=false;
    if(changed) ok = write_all_users(path, arr, n);
    free(arr);
    return ok;
}

bool delete_user(const char* path, const char* key){
    User* arr=NULL; int n = read_all_users(path, &arr);
    if(n<=0){ free(arr); return false; }
    int j=0, removed=0;
    for(int i=0;i<n;i++){
        if(match_key(&arr[i], key)){ removed=1; continue; }
        arr[j++] = arr[i];
    }
    bool ok=false;
    if(removed) ok = write_all_users(path, arr, j);
    free(arr);
    return ok;
}

int search_user(const char* path, const char* key, User* out_list, int max_out){
    User* arr=NULL; int n = read_all_users(path, &arr);
    if(n<=0){ free(arr); return 0; }

    int found=0;
    char key_lower[NAME_LEN];
    strncpy(key_lower, key, NAME_LEN); key_lower[NAME_LEN-1]='\0';
    for(char* c=key_lower; *c; ++c) *c = tolower((unsigned char)*c);

    for(int i=0;i<n;i++){
        char name_lower[NAME_LEN];
        strncpy(name_lower, arr[i].name, NAME_LEN); name_lower[NAME_LEN-1]='\0';
        for(char* c=name_lower; *c; ++c) *c = tolower((unsigned char)*c);

        if(strstr(name_lower, key_lower) || strcmp(arr[i].phone, key)==0){
            if(out_list && found<max_out) out_list[found] = arr[i];
            found++;
        }
    }
    free(arr);
    return found;
}

void display_menu(void){
    printf("\n==== User Management ====\n");
    printf("1) Add user\n");
    printf("2) Search user\n");
    printf("3) Edit user\n");
    printf("4) Delete user\n");
    printf("5) Show all\n");
    printf("0) Exit\n");
    printf("Select: ");
}
