# ============================================================
#  ONLINE MEMBERSHIP MANAGEMENT SYSTEM - MAKEFILE
#  รองรับ Windows / macOS / Linux (ใช้ GCC Compiler)
# ============================================================

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
SRC = main.c csv_manager.c
OBJ = $(SRC:.c=.o)
TARGET = app.exe
TEST_TARGET = test_unit.exe
TEST_SRC = test_unit.c csv_manager.c

# ------------------------------------------------------------
# 1️⃣ คอมไพล์และรันโปรแกรมหลัก
# ------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

# ------------------------------------------------------------
# 2️⃣ คอมไพล์และรัน Unit Test
# ------------------------------------------------------------
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC)

# ------------------------------------------------------------
# 3️⃣ ล้างไฟล์ชั่วคราว
# ------------------------------------------------------------
clean:
	del /Q *.o *.exe *.tmp *.bak *.csv 2>nul || true
	@echo "✅ Clean completed."

# ------------------------------------------------------------
# 4️⃣ สร้างไฟล์ CSV เริ่มต้น (ใช้กรณีไม่มี users.csv)
# ------------------------------------------------------------
init:
	echo user_id,name,service,start_date,duration,expiry_date,status,email,phone,last_updated,checksum > users.csv
	@echo "📁 Created users.csv file."

# ------------------------------------------------------------
# 📋 คำสั่งใช้งาน:
# make          → คอมไพล์โปรแกรมหลัก
# make run      → รันโปรแกรม
# make test     → รัน Unit Test
# make clean    → ล้างไฟล์ build
# make init     → สร้าง users.csv เริ่มต้น
# ------------------------------------------------------------

