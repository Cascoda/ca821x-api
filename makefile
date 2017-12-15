TARGET = libca821x.a
LIBS = -lm
INCLUDEDIR = include/
SOURCEDIR = source/
TESTDIR = test/
TESTAPP = $(TESTDIR)test_app

.PHONY: default all clean

default: $(TARGET) $(TESTAPP)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SOURCEDIR)*.c))
HEADERS = $(wildcard $(INCLUDEDIR),*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDEDIR)

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(AR) rcs $(TARGET) $^

$(TESTAPP):
	$(CC) $(CFLAGS) -o $(TESTDIR)test.o -c $(TESTDIR)test.c -I $(INCLUDEDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(TESTDIR)test.o -o $(TESTAPP)

clean:
	-rm -f $(SOURCEDIR)*.o
	-rm -f $(TARGET)
	-rm -f $(TESTDIR)*.o
	-rm -f $(TESTAPP)
