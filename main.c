#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_manager.h"

static void show_all(const char* path){
    User* arr=NULL; int n=read_all_users(path,&arr);
    if(n<=0){printf("ไม่มีข้อมูล\n");free(arr);return;}
    display_table_header();
    for(int i=0;i<n;i++) display_user_row(&arr[i]);
    printf("└────┴──────────────────────┴─────────────────┴────────────┴──────────┴────────────┴──────────┴────────────────────────┴───────────────┴────────────────────┘\n");
    free(arr);
}

int main(){
    const char* csv="users.csv"; int c;
    while(1){
        display_menu();
        if(scanf("%d",&c)!=1) break; getchar();
        if(c==0) break;
        else if(c==1){
            User u={0};
            printf("Name: ");fgets(u.name,sizeof(u.name),stdin);trim(u.name);
            printf("Service: ");fgets(u.service,sizeof(u.service),stdin);trim(u.service);
            printf("Start Date (YYYY-MM-DD): ");fgets(u.start_date,sizeof(u.start_date),stdin);trim(u.start_date);
            printf("Duration: ");fgets(u.duration,sizeof(u.duration),stdin);trim(u.duration);
            printf("Expiry Date (YYYY-MM-DD): ");fgets(u.expiry_date,sizeof(u.expiry_date),stdin);trim(u.expiry_date);
            printf("Status: ");fgets(u.status,sizeof(u.status),stdin);trim(u.status);
            printf("Email: ");fgets(u.email,sizeof(u.email),stdin);trim(u.email);
            printf("Phone: ");fgets(u.phone,sizeof(u.phone),stdin);trim(u.phone);
            add_user(csv,&u);
        }else if(c==2){
            char key[100];printf("Search key: ");fgets(key,100,stdin);trim(key);
            User out[256];
            int n=search_user(csv,key,out,256);
            printf("พบ %d รายการ\n",n);
            if(n>0){display_table_header();for(int i=0;i<n;i++)display_user_row(&out[i]);}
        }else if(c==3){
            char key[64];printf("Edit key: ");fgets(key,64,stdin);trim(key);
            User u={0};
            printf("New Name: ");fgets(u.name,sizeof(u.name),stdin);trim(u.name);
            printf("New Service: ");fgets(u.service,sizeof(u.service),stdin);trim(u.service);
            printf("New Start Date: ");fgets(u.start_date,sizeof(u.start_date),stdin);trim(u.start_date);
            printf("New Duration: ");fgets(u.duration,sizeof(u.duration),stdin);trim(u.duration);
            printf("New Expiry Date: ");fgets(u.expiry_date,sizeof(u.expiry_date),stdin);trim(u.expiry_date);
            printf("New Status: ");fgets(u.status,sizeof(u.status),stdin);trim(u.status);
            printf("New Email: ");fgets(u.email,sizeof(u.email),stdin);trim(u.email);
            printf("New Phone: ");fgets(u.phone,sizeof(u.phone),stdin);trim(u.phone);
            edit_user(csv,key,&u);
        }else if(c==4){
            char key[64];printf("Delete key: ");fgets(key,64,stdin);trim(key);
            User preview;
            if(search_user(csv,key,&preview,1)>0){
                printf("\n ข้อมูลที่จะลบ:\n");
                display_table_header();
                display_user_row(&preview);
            }
            if(!confirm_action("ยืนยันการลบ?")){
                printf("ยกเลิก\n");
                continue;
            }
            delete_user(csv,key);
        }else if(c==5) show_all(csv);
        else if(c==6) run_builtin_unit_tests(csv);
    }
    printf("Bye!\n");
    return 0;
}
