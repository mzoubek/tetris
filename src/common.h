#ifndef COMMON_H
#define COMMON_H

// Included libraries
#include <SDL2/SDL_error.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>

// Defines
#define ARENA_WIDTH_PX 400U
#define ARENA_HEIGHT_PX 800U
#define SCREEN_WIDTH_PX 1200U
#define SCREEN_HEIGHT_PX 800U
#define ARENA_PADDING_PX 400U
#define ARENA_WIDTH 8U
#define ARENA_HEIGHT 18U
#define ARENA_SIZE (ARENA_WIDTH * ARENA_HEIGHT)
#define PIECE_WIDTH 4U
#define PIECE_HEIGHT 4U
#define PIECE_SIZE 16U
#define TETROMINOS_DATA_SIZE 16U
#define BLOCK_SIZE_PX 50U
#define TETROMINOS_COUNT 7U
#define PIECE_COLOR_SIZE 4U
#define ARENA_PADDING_TOP 2U
#define FONT "./resources/retro_gaming.ttf"
#define MAX_HIGH_SCORES 10

// Enums
typedef enum {
    MENU_STATE,
    GAME_STATE,
    HIGHSCORES_STATE,
    CONTROLS_STATE,
    QUIT_STATE
} GameState;

enum {
    PIECE_I, PIECE_J, PIECE_L, PIECE_O, PIECE_S, PIECE_T,
    PIECE_Z, PIECE_COUNT
};

enum {
    COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_ORANGE,
    COLOR_GREY, COLOR_BLACK, COLOR_SIZE
};

enum {
    COLLIDE_NONE = 0,
    COLLIDE_LEFT = 1 << 0,
    COLLIDE_RIGHT = 1 << 1,
    COLLIDE_TOP = 1 << 2,
    COLLIDE_BOTTOM = 1 << 3,
    COLLIDE_PIECE = 1 << 4,
};

enum {
    UPDATE_MAIN,
    UPDATE_LOSE
};

// Structs
typedef struct _Size {
    int w;
    int h;
    uint8_t start_x;
    uint8_t start_y;
} Size;

typedef struct {
    char name[4];
    int score;
} HighScore;

typedef struct _Game {
    SDL_Texture *logo_texture;
    uint8_t level;
    uint64_t score;
    SDL_Renderer *renderer;
    SDL_Window *window;
    TTF_Font *lose_font;
    TTF_Font *ui_font;
    uint8_t placed[ARENA_SIZE];
    bool is_running;
    GameState state;
    uint8_t next_piece[PIECE_SIZE];
    uint8_t next_piece_color;
    HighScore high_scores[MAX_HIGH_SCORES];
} Game;

typedef struct {
    SDL_Rect rect; // Button area
    const char *label; // Button text
} MenuButton;

// Function pointer for updates
typedef uint8_t (*Update_callback)(Game *game, uint64_t frame, SDL_KeyCode key, bool keydown);

// Function prototypes
// game.c
void Game_Init(Game *game);
void Game_Update(Game *game, uint8_t fps);
void Game_Quit(Game *game);
void runGameLoop(Game *game, uint8_t fps, SDL_Event *event);

// logic.c
uint8_t collisionCheck(uint8_t *placed, uint8_t *piece, SDL_Point position);
void pickPiece(uint8_t *piece, uint8_t *color);
void rotatePiece(uint8_t *piece, uint8_t *rotated);
uint8_t checkForRowClearing(uint8_t *placed);
int findPoints(uint8_t level, uint8_t lines);
void addToPlaced(uint8_t *placed, uint8_t *piece, SDL_Point position);
void getPieceSize(uint8_t *piece, Size *size);
void getXY(uint8_t i, int *x, int *y);

// render.c
void drawTetromino(SDL_Renderer *renderer, uint8_t piece[PIECE_SIZE], SDL_Point position, uint8_t color);
void drawPlaced(uint8_t *placed, SDL_Renderer *renderer);
void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Point point);
void drawTextBlack(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Point point);
void renderGameInfo(SDL_Renderer *renderer, TTF_Font *font, uint64_t score, uint8_t level, uint8_t lines);
void drawNextTetromino(SDL_Renderer *renderer, uint8_t next_piece[PIECE_SIZE], uint8_t color);
void renderGameOver(Game *game);
void renderHighscores(SDL_Renderer *renderer, TTF_Font *font, HighScore *high_scores, SDL_Rect *back_button);
void renderControls(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect *back_button);
void renderMenu(Game *game, SDL_Renderer *renderer, TTF_Font *font, MenuButton *menu_buttons, int menu_button_count);
void renderGameElements(Game *game, uint8_t *current_piece, SDL_Point piece_position, uint8_t *placed,
                        uint8_t color, uint64_t score, uint8_t level, uint8_t lines_cleared);
void setColor(SDL_Renderer *renderer, uint8_t color);

// handlers.c
void handleControlsInput(Game *game, SDL_Event *event, SDL_Rect *back_button);
void handleHighscoresInput(Game *game, SDL_Event *event, SDL_Rect *back_button);
void handleMenuInput(Game *game, SDL_Event *event, MenuButton *menu_buttons, int menu_button_count);
void handleGameLogic(Game *game, uint8_t *current_piece, uint8_t *color, SDL_Point *piece_position,
                     uint8_t *placed, uint64_t *score, uint8_t *level, uint8_t *lines_cleared,
                     uint64_t *frame, bool *game_over);
void handleGameInput(Game *game, SDL_Event *event, uint8_t *current_piece, SDL_Point *piece_position, 
                     uint8_t *placed, uint8_t *color, bool *game_over);

// highscores.c
void saveHighScores(const char *filename, HighScore *high_scores);
void loadHighScores(const char *filename, HighScore *high_scores);
void addHighScore(HighScore *high_scores, const char *name, int score);
bool isHighScore(int score, HighScore *high_scores);
void getPlayerName(SDL_Renderer *renderer, TTF_Font *font, char *name);

// also from logic/render: helper functions

#endif
