TARGET = libcascoda.a
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -pthread
INCLUDEDIR = include/
SOURCEDIR = source/

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SOURCEDIR)*.c))
HEADERS = $(wildcard $(INCLUDEDIR),*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDEDIR)

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ar rcs $(TARGET) $^

clean:
	-rm -f $(SOURCEDIR)*.o
	-rm -f $(TARGET)