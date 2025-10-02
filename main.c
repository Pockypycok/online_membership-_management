#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_manager.h"

static void print_sub(const Subscriber* s){
    printf("- %s | %s | %s | %s\n", s->name, s->service, s->start_date, s->duration);
}

static void show_all(const char* path){
    Subscriber* arr=NULL; int n = read_all_users(path, &arr);
    if(n<=0){ printf("(ไม่มีข้อมูล)\n"); free(arr); return; }
    for(int i=0;i<n;i++) print_sub(&arr[i]);
    free(arr);
}

int main(int argc, char** argv){
    const char* csv_path = (argc>1) ? argv[1] : "subscriber.csv";
    int choice;

    while(1){
        display_menu();
        if(scanf("%d", &choice)!=1){ printf("อินพุตไม่ถูกต้อง\n"); return 0; }
        getchar();

        if(choice==0) break;
        else if(choice==1){
            Subscriber s;
            printf("ชื่อผู้สมัคร: "); fgets(s.name, sizeof(s.name), stdin); trim(s.name);
            printf("บริการที่สมัคร: "); fgets(s.service, sizeof(s.service), stdin); trim(s.service);
            printf("วันที่สมัคร (YYYY-MM-DD): "); fgets(s.start_date, sizeof(s.start_date), stdin); trim(s.start_date);
            printf("ระยะเวลาสมัคร: "); fgets(s.duration, sizeof(s.duration), stdin); trim(s.duration);
            add_user(csv_path,&s);
        }else if(choice==2){
            char key[128]; printf("คำค้นหา: "); fgets(key, sizeof(key), stdin); trim(key);
            Subscriber out[50]; int n=search_user(csv_path,key,out,50);
            printf("พบ %d รายการ\n",n); for(int i=0;i<n;i++) print_sub(&out[i]);
        }else if(choice==3){
            char key[128]; printf("แก้ไขใคร: "); fgets(key, sizeof(key), stdin); trim(key);
            Subscriber s; printf("ชื่อใหม่: "); fgets(s.name,96,stdin); trim(s.name);
            printf("บริการใหม่: "); fgets(s.service,96,stdin); trim(s.service);
            printf("วันที่ใหม่: "); fgets(s.start_date,32,stdin); trim(s.start_date);
            printf("ระยะเวลาใหม่: "); fgets(s.duration,32,stdin); trim(s.duration);
            edit_user(csv_path,key,&s);
        }else if(choice==4){
            char key[128]; printf("ลบใคร: "); fgets(key,sizeof(key),stdin); trim(key);
            delete_user(csv_path,key);
        }else if(choice==5){
            show_all(csv_path);
        }
    }
    return 0;
}
    
