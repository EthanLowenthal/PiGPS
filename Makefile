# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:

CFLAGS  = -std=c++17 -Wall
LIBS = cairomm-1.0 libgps
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).c
			$(CC) $(pkg-config --cflags --libs LIBS) $(CFLAGS) -o $(TARGET) src/$(TARGET).cpp

clean:
			$(RM) $(TARGET)