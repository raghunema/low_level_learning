CC = clang
CFLAGS = -Wall -Wextra -g
TARGET = Day1Puzzle1
SOURCE = Day1Puzzle1.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
