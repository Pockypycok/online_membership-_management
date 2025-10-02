CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2

APP_SOURCES=main.c csv_manager.c
TEST_SOURCES=tests/test_unit.c csv_manager.c

all: app

app: $(APP_SOURCES)
	$(CC) $(CFLAGS) -o app $(APP_SOURCES)

test: $(TEST_SOURCES)
	$(CC) $(CFLAGS) -o test_unit $(TEST_SOURCES)

clean:
	rm -f app test_unit
