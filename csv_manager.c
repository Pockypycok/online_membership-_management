#include "csv_manager.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void trim(char* s){
    if(!s) return;
    size_t i=0; while(s[i] && isspace((unsigned char)s[i])) i++;
    if(i) memmove(s, s+i, strlen(s+i)+1);
    size_t n=strlen(s);
    while(n>0 && isspace((unsigned char)s[n-1])) s[--n]='\0';
}

FILE* open_file(const char* path, const char* mode){
    return fopen(path, mode);
}

static int is_header_line(const char* line){
    return strncmp(line,"ชื่อผู้สมัคร,",20)==0;
}

static int parse_line(const char* line, Subscriber* s){
    return sscanf(line, " %95[^,],%95[^,],%31[^,],%31[^\n\r]",
                  s->name, s->service, s->start_date, s->duration) == 4;
}

static void format_line(char* buf, size_t buflen, const Subscriber* s){
    snprintf(buf, buflen, "%s,%s,%s,%s\n",
             s->name, s->service, s->start_date, s->duration);
}

int read_all_users(const char* path, Subscriber** out_list){
    *out_list=NULL; FILE* f=open_file(path,"r"); if(!f) return 0;
    char line[512]; int cap=16,cnt=0; Subscriber* arr=malloc(sizeof(Subscriber)*cap);
    while(fgets(line,sizeof(line),f)){
        if(is_header_line(line)) continue;
        Subscriber s; if(parse_line(line,&s)){
            if(cnt>=cap){ cap*=2; arr=realloc(arr,sizeof(Subscriber)*cap); }
            arr[cnt++]=s;
        }
    }
    fclose(f); *out_list=arr; return cnt;
}

bool write_all_users(const char* path, const Subscriber* list, int count){
    FILE* f=open_file(path,"w"); if(!f) return false;
    fprintf(f,"ชื่อผู้สมัคร,บริการที่สมัคร,วันที่สมัคร,ระยะเวลาสมัคร\n");
    for(int i=0;i<count;i++){ char buf[256]; format_line(buf,sizeof(buf),&list[i]); fputs(buf,f); }
    fclose(f); return true;
}

bool add_user(const char* path, const Subscriber* s){
    FILE* f=fopen(path,"r");
    if(!f){ FILE* fw=fopen(path,"w");
        fprintf(fw,"ชื่อผู้สมัคร,บริการที่สมัคร,วันที่สมัคร,ระยะเวลาสมัคร\n"); fclose(fw);
    } else fclose(f);
    f=open_file(path,"a"); if(!f) return false;
    char buf[256]; format_line(buf,sizeof(buf),s); fputs(buf,f); fclose(f);
    return true;
}

static int match_key(const Subscriber* s,const char* key){
    return strstr(s->name,key)||strstr(s->service,key)||strstr(s->start_date,key)||strstr(s->duration,key);
}

bool edit_user(const char* path,const char* key,const Subscriber* updated){
    Subscriber* arr; int n=read_all_users(path,&arr); int done=0;
    for(int i=0;i<n;i++){ if(match_key(&arr[i],key)){ arr[i]=*updated; done=1; break; } }
    if(done) write_all_users(path,arr,n);
    free(arr); return done;
}

bool delete_user(const char* path,const char* key){
    Subscriber* arr; int n=read_all_users(path,&arr); int j=0,removed=0;
    for(int i=0;i<n;i++){ if(match_key(&arr[i],key)){removed=1;continue;} arr[j++]=arr[i]; }
    if(removed) write_all_users(path,arr,j);
    free(arr); return removed;
}

int search_user(const char* path,const char* key,Subscriber* out,int max_out){
    Subscriber* arr; int n=read_all_users(path,&arr),found=0;
    for(int i=0;i<n;i++) if(match_key(&arr[i],key)){ if(out&&found<max_out) out[found]=arr[i]; found++; }
    free(arr); return found;
}

void display_menu(void){
    printf("\n==== ระบบจัดการการสมัครบริการ ====\n");
    printf("1) เพิ่ม\n2) ค้นหา\n3) แก้ไข\n4) ลบ\n5) แสดงทั้งหมด\n0) ออก\nเลือกเมนู: ");
}

