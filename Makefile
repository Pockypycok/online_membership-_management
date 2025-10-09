# ============================================================
#  ONLINE MEMBERSHIP MANAGEMENT SYSTEM - MAKEFILE
# ============================================================

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
SRC = main.c csv_manager.c
OBJ = $(SRC:.c=.o)
TARGET = app.exe
TEST_TARGET = test_unit.exe
TEST_SRC = test_unit.c csv_manager.c

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC)

clean:
	del /Q *.o *.exe *.tmp *.bak *.csv 2>nul || true
	@echo "Clean completed."

init:
	echo user_id,name,service,start_date,duration,expiry_date,status,email,phone,last_updated,checksum > users.csv
	@echo "Created users.csv file."


