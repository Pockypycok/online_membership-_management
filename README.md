# online_membership-_management
user subscription
# ระบบจัดการสมาชิกด้วยภาษา C

โครงการนี้เป็นโปรแกรม C สำหรับจัดการสมาชิกในไฟล์ CSV

## ไฟล์ที่ประกอบด้วย
- `main.c` : โปรแกรมหลัก มีเมนูเพิ่ม/ค้นหา/แก้ไข/ลบ/แสดงข้อมูล
- `Test.c` : โค้ดจัดการ CSV (add, edit, delete, search, read, write)
- `Test_esc.c` : struct `User` และ function prototypes
- `Member.csv` : ไฟล์ข้อมูลตัวอย่าง (≥15 รายการ)
- `test_unit.c` : unit test 2 เคส
- `run_e2e.sh` : สคริปต์ทดสอบ end-to-end
- `Makefile` : สคริปต์คอมไพล์

## วิธีคอมไพล์
```bash
make
