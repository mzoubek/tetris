#include "common.h"

void handleControlsInput(Game *game, SDL_Event *event, SDL_Rect *back_button) {
    if (event->type == SDL_QUIT) {
        game->is_running = false; // Exit the game
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;

        // Check if the Back button is clicked
        if (x >= back_button->x && x <= back_button->x + back_button->w &&
            y >= back_button->y && y <= back_button->y + back_button->h) {
            game->state = MENU_STATE;
        }
    } else if (event->type == SDL_KEYDOWN) {
        // Allow pressing Escape to go back to the menu
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            game->state = MENU_STATE;
        }
    }
}

void handleHighscoresInput(Game *game, SDL_Event *event, SDL_Rect *back_button) {
    if (event->type == SDL_QUIT) {
        game->is_running = false; // Exit the game
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;

        // Check if the Back button is clicked
        if (x >= back_button->x && x <= back_button->x + back_button->w &&
            y >= back_button->y && y <= back_button->y + back_button->h) {
            game->state = MENU_STATE;
        }
    } else if (event->type == SDL_KEYDOWN) {
        // Allow pressing Escape to go back to the menu
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            game->state = MENU_STATE;
        }
    }
}

void handleMenuInput(Game *game, SDL_Event *event, MenuButton *menu_buttons, int menu_button_count) {
    if (event->type == SDL_QUIT) {
        game->is_running = false;
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;

        for (int i = 0; i < menu_button_count; i++) {
            if (x >= menu_buttons[i].rect.x &&
                x <= menu_buttons[i].rect.x + menu_buttons[i].rect.w &&
                y >= menu_buttons[i].rect.y &&
                y <= menu_buttons[i].rect.y + menu_buttons[i].rect.h) {
                // Handle button click
                switch (i) {
                    case 0: // New Game
                        game->state = GAME_STATE;
                        break;
                    case 1: // Highest Scores
                        game->state = HIGHSCORES_STATE;
                        break;
                    case 2: // Controls
                        game->state = CONTROLS_STATE;
                        break;
                    case 3: // Quit
                        game->is_running = false;
                        break;
                }
            }
        }
    }
}

void handleGameInput(Game *game, SDL_Event *event, uint8_t *current_piece, SDL_Point *piece_position, 
                     uint8_t *placed, uint8_t *color, bool *game_over) {
    if (event->type == SDL_QUIT) {
        game->is_running = false; // Exit the game
    } else if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_a: // Move piece left
                piece_position->x--;
                if (collisionCheck(placed, current_piece, *piece_position)) {
                    piece_position->x++; 
                }
                break;

            case SDLK_d: // Move piece right
                piece_position->x++;
                if (collisionCheck(placed, current_piece, *piece_position)) {
                    piece_position->x--;
                }
                break;

            case SDLK_s: // Soft drop (move piece down)
                piece_position->y++;
                if (collisionCheck(placed, current_piece, *piece_position)) {
                    piece_position->y--;
                    addToPlaced(placed, current_piece, *piece_position);

                    // Transition to the next piece
                    memcpy(current_piece, game->next_piece, sizeof(uint8_t) * PIECE_SIZE);
                    *color = game->next_piece_color;

                    // Generate a new next piece
                    pickPiece(game->next_piece, &game->next_piece_color);

                    *piece_position = (SDL_Point){.x = ARENA_WIDTH / 2 - 2, .y = 0};

                    // Check if the new piece immediately collides (game over)
                    if (collisionCheck(placed, current_piece, *piece_position)) {
                        *game_over = true;
                    }
                }
                break;

            case SDLK_r: { // Rotate piece
                uint8_t rotated[PIECE_SIZE] = {0};
                rotatePiece(current_piece, rotated);
                if (!collisionCheck(placed, rotated, *piece_position)) {
                    memcpy(current_piece, rotated, sizeof(uint8_t) * PIECE_SIZE);
                }
                break;
            }
        }
    }
}

void handleGameLogic(Game *game, uint8_t *current_piece, uint8_t *color, SDL_Point *piece_position,
                     uint8_t *placed, uint64_t *score, uint8_t *level, uint8_t *lines_cleared,
                     uint64_t *frame, bool *game_over) {
    // Automatic piece drop
    if (*frame % (30 / *level) == 0) {
        piece_position->y++;
        if (collisionCheck(placed, current_piece, *piece_position)) {
            piece_position->y--;
            addToPlaced(placed, current_piece, *piece_position);

            // Transition to the next piece
            memcpy(current_piece, game->next_piece, sizeof(uint8_t) * PIECE_SIZE);
            *color = game->next_piece_color;

            // Generate a new next piece
            pickPiece(game->next_piece, &game->next_piece_color);

            *piece_position = (SDL_Point){.x = ARENA_WIDTH / 2 - 2, .y = 0};

            // Check for immediate collision (game over)
            if (collisionCheck(placed, current_piece, *piece_position)) {
                *game_over = true;
            }
        }
    }

    // Check for row clearing
    uint8_t cleared = checkForRowClearing(placed);
    if (cleared > 0) {
        *lines_cleared += cleared;
        *score += findPoints(*level, cleared);
        if (*lines_cleared / 10 > *level - 1) {
            (*level)++;
        }
    }
}
