CC = gcc
CFLAGS = -Wall
LIBS = -lsqlite3
TARGET = myprogram

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
