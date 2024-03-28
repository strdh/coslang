CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = 
TARGET = coslang

HEADER = $(wildcard *.h)
SOURCE = $(wildcard *.c)
OBJECTS = $(SOURCE:.c=.o)

INCLUDES = -I.

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean target
.PHONY: clean
clean:
	del $(OBJECTS) $(TARGET)
