#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../csv_manager.h"

static const char* TMP="tests/tmp.csv";

void reset(){
    FILE* f=fopen(TMP,"w");
    fprintf(f,"ชื่อผู้สมัคร,บริการที่สมัคร,วันที่สมัคร,ระยะเวลาสมัคร\n");
    fprintf(f,"ทดสอบ1,Netflix,2025-01-01,12 เดือน\n");
    fclose(f);
}

void test_add_search(){
    reset();
    Subscriber s={"ทดสอบ2","Spotify","2025-02-01","6 เดือน"};
    assert(add_user(TMP,&s));
    Subscriber out[10];
    int n=search_user(TMP,"ทดสอบ2",out,10);
    assert(n==1);
}

void test_edit_delete(){
    reset();
    Subscriber s={"ทดสอบ1แก้ไข","Disney+","2025-02-01","3 เดือน"};
    assert(edit_user(TMP,"ทดสอบ1",&s));
    Subscriber out[10];
    int n=search_user(TMP,"แก้ไข",out,10);
    assert(n==1);
    assert(delete_user(TMP,"แก้ไข"));
    n=search_user(TMP,"แก้ไข",out,10);
    assert(n==0);
}

int main(){
    test_add_search();
    test_edit_delete();
    printf("Unit tests passed.\n");
    return 0;
}
