CC = gcc
CFLAGS = -Wall
LIBS = -lsqlite3
TARGET = appointment

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) appointment.c -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
