APIINCLUDEDIR = ../include
SOURCEDIR = .

OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SOURCEDIR),*.c))
HEADERS = $(wildcard $(APIINCLUDEDIR),*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(APIINCLUDEDIR)

all: $(OBJECTS)