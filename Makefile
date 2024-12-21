PROG = tetris
CC = gcc
CFLAGS = `sdl2-config --cflags` -Wall
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
SRC = src/main.c
OBJ = $(SRC:.c=.o)

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(PROG)
