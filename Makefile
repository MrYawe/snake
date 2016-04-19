# project name (generate executable with this name)
TARGET   = larasnake

CC       = gcc
# compiling flags here
CFLAGS   = -Wall -I. `sdl-config --libs` -lSDL_image -lSDL_gfx

LINKER   = gcc -o
# linking flags here
LFLAGS   = -I. -lm `sdl-config --libs` -lSDL_image -lSDL_gfx

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = build
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@test -d $(BINDIR) || mkdir -p $(BINDIR)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	@$(CC) -c $< -o $@ $(CFLAGS)
	@echo "Compiled "$<" successfully!"

.PHONEY: test
test: build/board.o build/testAll.o build/testBoard.o
	gcc -o bin/testAll build/board.o build/testBoard.o build/testAll.o -l cmocka

build/testAll.o: build/testBoard.o build/testSnake.o build/testIa.o
	gcc -o build/testAll.o -c tests/testAll.c -l cmocka

build/testBoard.o: src/board.h tests/testBoard.h tests/testBoard.c
	gcc -o build/testBoard.o -c tests/testBoard.c -l cmocka

build/testSnake.o: src/snake.h tests/testSnake.h tests/testSnake.c
	gcc -o build/testSnake.o -c tests/testSnake.c -l cmocka

build/testIa.o: src/ia.h tests/testIa.h tests/testIa.c
	gcc -o build/testIa.o -c tests/testIa.c -l cmocka


.PHONEY: clean
clean:
	@$(rm) $(OBJDIR)/*.o
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@$(rm) $(BINDIR)/testAll
	@echo "Executable removed!"
