CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic
LFLAGS = -lncurses
SOURCES = $(wildcard *.c)
EXEC = life
OBJECTS = $(SOURCES:%.c=%.o)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $<
tidy:
	rm -rf $(OBJECTS)
clean: tidy
	rm -rf $(EXEC)

format:
	clang-format -i -style=file *.[c,h]
