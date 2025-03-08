#include "common.h"

uint8_t collisionCheck(uint8_t *placed, uint8_t *piece, SDL_Point position) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        if (piece[i]) {
            int x, y;
            getXY(i, &x, &y);
            x += position.x;
            y += position.y;

            // Check bounds
            if (x < 0 || x >= ARENA_WIDTH || y >= ARENA_HEIGHT) {
                return 1; // Collision with bounds
            }

            // Check existing blocks
            if (y >= 0 && placed[y * ARENA_WIDTH + x]) {
                return 1; // Collision with placed blocks
            }
        }
    }
    return 0; // No collision
}

void pickPiece(uint8_t *piece, uint8_t *color)
{
    static const uint8_t tetrominos[TETROMINOS_COUNT][PIECE_SIZE] = {
        // I
        {0,0,0,0,
         1,1,1,1,
         0,0,0,0,
         0,0,0,0},
        
        // J
        {0,0,0,0,
         1,0,0,0,
         1,1,1,0,
         0,0,0,0},

        // L
        {0,0,0,0,
         0,0,1,0,
         1,1,1,0,
         0,0,0,0},

        // O
        {0,0,0,0,
         0,1,1,0,
         0,1,1,0,
         0,0,0,0},

        // S
        {0,0,0,0,
         0,1,1,0,
         1,1,0,0,
         0,0,0,0},
        
        // T
        {0,0,0,0,
         0,1,0,0,
         1,1,1,0,
         0,0,0,0},
        
        // Z
        {0,0,0,0,
         1,1,0,0,
         0,1,1,0,
         0,0,0,0},
    };

    static const uint8_t piece_colors[PIECE_COLOR_SIZE] = {
        COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_ORANGE
    };

    // uint8_t id = (uint8_t)((float)rand() / (float)RAND_MAX * TETROMINOS_COUNT);
    uint8_t id = rand() % TETROMINOS_COUNT;
    memcpy(piece, tetrominos[id], PIECE_SIZE);

    *color = piece_colors[((*color)+1) % PIECE_COLOR_SIZE];
}

void rotatePiece(uint8_t *piece, uint8_t *rotated) {
    memset(rotated, 0, sizeof(uint8_t) * PIECE_SIZE); // Initialize the rotated array

    for (int i = 0; i < PIECE_HEIGHT; i++) {
        for (int j = 0; j < PIECE_WIDTH; j++) {
            rotated[j * PIECE_WIDTH + (PIECE_WIDTH - 1 - i)] = piece[i * PIECE_WIDTH + j];
        }
    }
}

uint8_t checkForRowClearing(uint8_t *placed) {
    int cleared_rows = 0;
    for (int y = 0; y < ARENA_HEIGHT; y++) {
        int filled = 1;
        for (int x = 0; x < ARENA_WIDTH; x++) {
            if (!placed[y * ARENA_WIDTH + x]) {
                filled = 0;
                break;
            }
        }
        if (filled) {
            cleared_rows++;
            // Move rows down
            for (int k = y; k > 0; k--) {
                for (int x = 0; x < ARENA_WIDTH; x++) {
                    placed[k * ARENA_WIDTH + x] = placed[(k - 1) * ARENA_WIDTH + x];
                }
            }
            // Clear top row
            for (int x = 0; x < ARENA_WIDTH; x++) {
                placed[x] = 0;
            }
        }
    }
    return cleared_rows;
}

void addToPlaced(uint8_t *placed, uint8_t *piece, SDL_Point position) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        if (piece[i]) {
            int x, y;
            getXY(i, &x, &y);
            x += position.x;
            y += position.y;

            if (y >= 0 && y < ARENA_HEIGHT && x >= 0 && x < ARENA_WIDTH) {
                placed[y * ARENA_WIDTH + x] = piece[i]; // Add block to arena
            }
        }
    }
}

void getPieceSize(uint8_t *piece, Size *size) {
    size->w = 0;
    size->h = 0;
    size->start_x = PIECE_WIDTH;
    size->start_y = PIECE_HEIGHT;

    for (int i = 0; i < PIECE_SIZE; i++) {
        if (piece[i]) {
            int x = i % PIECE_WIDTH;
            int y = i / PIECE_WIDTH;
            if (x < size->start_x) size->start_x = x;
            if (x >= size->w) size->w = x + 1;
            if (y < size->start_y) size->start_y = y;
            if (y >= size->h) size->h = y + 1;
        }
    }
}

void getXY(uint8_t index, int *x, int *y) {
    *x = index % PIECE_WIDTH; // Column index
    *y = index / PIECE_WIDTH; // Row index
}

int findPoints(uint8_t level, uint8_t lines) {
    static const int points_per_line[] = {0, 40, 100, 300, 1200};
    return points_per_line[lines] * (level + 1);
}
