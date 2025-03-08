##########################################
#        Configuration                   #
##########################################

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c99
CFLAGS  += -I src

# Use pkg-config (or sdl2-config, if installed)
SDL2_CFLAGS ?= $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_image)
SDL2_LIBS   ?= $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_image)

# Folders
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := .

# Target name (the final executable)
TARGET  := tetris

##########################################
#        Sources and Objects             #
##########################################

# Find all .c files under SRC_DIR
SOURCES := $(wildcard $(SRC_DIR)/*.c)
# For each .c file, create a matching .o in OBJ_DIR
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

##########################################
#        Primary Build Targets           #
##########################################

# Default target: build the executable
all: $(BIN_DIR)/$(TARGET)

# Link the final binary
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(SDL2_LIBS)
	@echo "Linking complete: $@"

# Compile each .c into an .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

##########################################
#        Chores                          #
##########################################

# Remove all object files and the binary
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/$(TARGET)
	@echo "Cleaned all build artifacts."

# Convenience target to clean and then rebuild
rebuild: clean all

.PHONY: all clean rebuild
