#include "csv_manager.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

static void memzero(void* p, size_t n){ memset(p, 0, n); }
void trim(char* s){ if(!s)return; size_t i=0; while(s[i] && (unsigned char)s[i]<=32)i++; if(i)memmove(s,s+i,strlen(s+i)+1); size_t n=strlen(s); while(n>0&&(unsigned char)s[n-1]<=32)s[--n]='\0'; }
void set_now(char out[NOW_LEN]){ time_t t=time(NULL); struct tm*lt=localtime(&t); snprintf(out,NOW_LEN,"%04d-%02d-%02dT%02d:%02d:%02d",lt->tm_year+1900,lt->tm_mon+1,lt->tm_mday,lt->tm_hour,lt->tm_min,lt->tm_sec); }

unsigned long long compute_checksum(const User* u){
    const unsigned long long FNV_OFFSET=0xcbf29ce484222325ULL, FNV_PRIME=0x100000001b3ULL;
    unsigned long long h=FNV_OFFSET; #define FEED(s) do{const unsigned char*P=(const unsigned char*)(s);for(size_t i=0;i<strlen(s);i++){h^=P[i];h*=FNV_PRIME;}}while(0)
    char buf[32]; snprintf(buf,sizeof(buf),"%d",u->user_id);
    FEED(buf); FEED(u->name); FEED(u->service); FEED(u->start_date); FEED(u->duration);
    FEED(u->expiry_date); FEED(u->status); FEED(u->email); FEED(u->phone); FEED(u->last_updated);
    #undef FEED
    return h;
}

bool validate_user(const User* u, char err[], size_t len){
    if(strlen(u->name)==0){snprintf(err,len,"ชื่อห้ามว่าง");return false;}
    if(strchr(u->email,'@')==NULL){snprintf(err,len,"อีเมลไม่ถูกต้อง");return false;}
    if(strlen(u->phone)<7){snprintf(err,len,"เบอร์โทรสั้นเกินไป");return false;}
    return true;
}

int generate_next_id(const char* path){
    User* a=NULL; int n=read_all_users(path,&a); int max=0;
    for(int i=0;i<n;i++) if(a[i].user_id>max) max=a[i].user_id;
    free(a); return max+1;
}

void update_auto_status(User* u){
    time_t t=time(NULL); struct tm*lt=localtime(&t);
    char today[DATE_LEN]; snprintf(today,DATE_LEN,"%04d-%02d-%02d",lt->tm_year+1900,lt->tm_mon+1,lt->tm_mday);
    if(strcmp(today,u->expiry_date)>0) strcpy(u->status,"Expired");
    else if(strlen(u->status)==0) strcpy(u->status,"Active");
}

int read_all_users(const char* path, User** out){
    *out=NULL; FILE* f=fopen(path,"r"); if(!f) return 0;
    char line[1024]; int cap=32,cnt=0; User*arr=malloc(sizeof(User)*cap);
    while(fgets(line,sizeof(line),f)){
        if(strncmp(line,"user_id",7)==0) continue;
        User u={0}; unsigned long long sum=0;
        sscanf(line,"%d,%99[^,],%99[^,],%15[^,],%31[^,],%15[^,],%15[^,],%127[^,],%31[^,],%19[^,],%llu",
               &u.user_id,u.name,u.service,u.start_date,u.duration,u.expiry_date,u.status,u.email,u.phone,u.last_updated,&sum);
        u.checksum=sum; if(u.checksum!=compute_checksum(&u)) continue;
        update_auto_status(&u);
        if(cnt>=cap){cap*=2; arr=realloc(arr,sizeof(User)*cap);}
        arr[cnt++]=u;
    }
    fclose(f); *out=arr; return cnt;
}

bool write_all_users_safe(const char* path,const User* list,int n){
    char tmp[256]; snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE*f=fopen(tmp,"w"); if(!f)return false;
    fprintf(f,"user_id,name,service,start_date,duration,expiry_date,status,email,phone,last_updated,checksum\n");
    for(int i=0;i<n;i++){User u=list[i]; u.checksum=compute_checksum(&u);
        fprintf(f,"%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%llu\n",u.user_id,u.name,u.service,u.start_date,u.duration,u.expiry_date,u.status,u.email,u.phone,u.last_updated,u.checksum);}
    fclose(f); remove(path); rename(tmp,path); return true;
}

bool add_user(const char* path,const User* in){
    char err[128]; if(!validate_user(in,err,sizeof(err))){printf(RED "%s\n" RESET,err);return false;}
    User u=*in; if(u.user_id==0) u.user_id=generate_next_id(path);
    update_auto_status(&u); set_now(u.last_updated); u.checksum=compute_checksum(&u);
    User*a=NULL;int n=read_all_users(path,&a);
    a=realloc(a,sizeof(User)*(n+1)); a[n]=u;
    bool ok=write_all_users_safe(path,a,n+1); free(a);
    if(ok) printf(GREEN "เพิ่มข้อมูลสำเร็จ\n" RESET);
    return ok;
}

bool edit_user(const char* path,const char* key,const User* upd){
    User*a=NULL;int n=read_all_users(path,&a);int done=0;
    for(int i=0;i<n;i++) if(strstr(a[i].name,key)){User u=*upd;u.user_id=a[i].user_id;set_now(u.last_updated);update_auto_status(&u);a[i]=u;done=1;break;}
    bool ok=false; if(done) ok=write_all_users_safe(path,a,n);
    free(a);
    if(ok) printf(GREEN "แก้ไขข้อมูลเรียบร้อย\n" RESET);
    else printf(RED "ไม่พบข้อมูล\n" RESET);
    return ok;
}

bool confirm_action(const char* msg){ char a[8]; printf(YELLOW "%s (y/n): " RESET,msg); fgets(a,sizeof(a),stdin); return tolower(a[0])=='y'; }

bool recycle_deleted_user(const User* u){
    FILE* f=fopen("deleted_users.csv","a"); if(!f)return false;
    static int header=0; if(!header){fseek(f,0,SEEK_END); if(ftell(f)==0)
        fprintf(f,"user_id,name,service,start_date,duration,expiry_date,status,email,phone,last_updated,checksum\n");
        header=1;}
    fprintf(f,"%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%llu\n",u->user_id,u.name,u.service,u.start_date,u.duration,u.expiry_date,u.status,u.email,u.phone,u.last_updated,u.checksum);
    fclose(f); return true;
}

bool delete_user(const char* path,const char* key){
    User*a=NULL;int n=read_all_users(path,&a);if(n==0){printf("ไม่มีข้อมูล\n");return false;}
    int idx=-1; for(int i=0;i<n;i++) if(strstr(a[i].name,key)||strstr(a[i].email,key)){idx=i;break;}
    if(idx==-1){printf(RED "ไม่พบข้อมูล\n" RESET);free(a);return false;}
    printf(CYAN "\n ข้อมูลที่จะลบ:\n" RESET); display_table_header(); display_user_row(&a[idx]);
    if(!confirm_action("ยืนยันการลบ?")){printf(YELLOW "ยกเลิก\n" RESET);free(a);return false;}
    recycle_deleted_user(&a[idx]);
    for(int i=idx;i<n-1;i++) a[i]=a[i+1];
    bool ok=write_all_users_safe(path,a,n-1);free(a);
    if(ok) printf(GREEN "ลบสำเร็จ\n" RESET);
    return ok;
}

void display_table_header(void){
    printf(BOLD "┌────┬──────────────────────┬─────────────────┬────────────┬──────────┬────────────┬──────────┬────────────────────────┬───────────────┬────────────────────┐\n");
    printf("│ ID │ Name                 │ Service         │ Start Date │ Duration │ Expiry     │ Status   │ Email                  │ Phone         │ Last Updated       │\n");
    printf("├────┼──────────────────────┼─────────────────┼────────────┼──────────┼────────────┼──────────┼────────────────────────┼───────────────┼────────────────────┤\n" RESET);
}
void display_user_row(const User*u){
    printf("│%3d │ %-20.20s │ %-15.15s │ %-10.10s │ %-8.8s │ %-10.10s │ %-8.8s │ %-22.22s │ %-13.13s │ %-19.19s │\n",
           u->user_id,u->name,u->service,u->start_date,u->duration,u->expiry_date,u->status,u->email,u->phone,u->last_updated);
}

void display_menu(void){
    printf(BOLD CYAN "\n╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║                 ONLINE MEMBERSHIP MANAGEMENT SYSTEM                  ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════╣\n");
    printf("║ 1) Add User                                                          ║\n");
    printf("║ 2) Search User                                                       ║\n");
    printf("║ 3) Edit User                                                         ║\n");
    printf("║ 4) Delete User                                                       ║\n");
    printf("║ 5) Display All                                                       ║\n");
    printf("║ 6) Run Built-in Unit Tests                                           ║\n");
    printf("║ 0) Exit                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════╝\n" RESET);
    printf(YELLOW "Select: " RESET);
}
