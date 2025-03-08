#include "common.h"

void setColor(SDL_Renderer *renderer, uint8_t color) {
    const SDL_Color colors[] = {
        [COLOR_RED] = { .r = 255, .g = 0, .b = 0, .a = 255 }, 
        [COLOR_GREEN] = { .r = 0, .g = 255, .b = 0, .a = 255 },
        [COLOR_BLUE] = { .r = 0, .g = 0, .b = 255, .a = 255 },
        [COLOR_ORANGE] = { .r = 255, .g = 165, .b = 0, .a = 255 },
        [COLOR_GREY] = { .r = 169, .g = 169, .b = 169, .a = 255 },
        [COLOR_BLACK] = { .r = 0, .g = 0, .b = 0, .a = 255 },
    };

    SDL_SetRenderDrawColor(renderer,
                           colors[color].r,
                           colors[color].g,
                           colors[color].b,
                           colors[color].a);
}

void drawTetromino(SDL_Renderer *renderer, uint8_t piece[PIECE_SIZE], SDL_Point position, uint8_t color)
{
    for (int i = 0; i < TETROMINOS_DATA_SIZE; i++) {
        if (!piece[i]) continue;

        int x, y;
        getXY(i, &x, &y);

        SDL_Rect rect = {
            .x = ((x + position.x) * BLOCK_SIZE_PX) + ARENA_PADDING_PX,
            .y = (y + position.y - ARENA_PADDING_TOP) * BLOCK_SIZE_PX,
            .w = BLOCK_SIZE_PX,
            .h = BLOCK_SIZE_PX
        };

        setColor(renderer, color);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void drawPlaced(uint8_t *placed, SDL_Renderer *renderer)
{
    for (int x = 0; x < ARENA_WIDTH; ++x) {
        for (int y = 0; y < ARENA_HEIGHT; ++y) {
            uint8_t i = y * ARENA_WIDTH + x;

            if (!placed[i]) continue;

            SDL_Rect rect = {
                .x = (x * BLOCK_SIZE_PX) + ARENA_PADDING_PX,
                .y = (y - ARENA_PADDING_TOP) * BLOCK_SIZE_PX,
                .w = BLOCK_SIZE_PX, .h = BLOCK_SIZE_PX
            };

            setColor(renderer, COLOR_GREY);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Point point) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, white);
    if (!surface) {
        fprintf(stderr, "ERROR: TTF_RenderText_Blended failed: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "ERROR: SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dest = {
        point.x,
        point.y,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    if (SDL_RenderCopy(renderer, texture, NULL, &dest) != 0) {
        fprintf(stderr, "ERROR: SDL_RenderCopy failed: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawTextBlack(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Point point) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, black);
    if (!surface) {
        fprintf(stderr, "ERROR: TTF_RenderText_Blended failed: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "ERROR: SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dest = {
        point.x,
        point.y,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    if (SDL_RenderCopy(renderer, texture, NULL, &dest) != 0) {
        fprintf(stderr, "ERROR: SDL_RenderCopy failed: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawCenteredText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Point point) {
    int text_width, text_height;
    TTF_SizeText(font, text, &text_width, &text_height);

    SDL_Point centered_point = {
        point.x - text_width / 2,
        point.y - text_height / 2
    };
    drawText(renderer, font, text, centered_point);
}

void renderGameInfo(SDL_Renderer *renderer, TTF_Font *font, uint64_t score, uint8_t level, uint8_t lines) {
    char buffer[64];

    // Score
    sprintf(buffer, "Score: %ld", score);
    SDL_Point score_position = {.x = 50, .y = 50};
    drawText(renderer, font, buffer, score_position);

    // Level
    sprintf(buffer, "Level: %d", level);
    SDL_Point level_position = {.x = 50, .y = 100};
    drawText(renderer, font, buffer, level_position);

    // Lines
    sprintf(buffer, "Lines: %d", lines);
    SDL_Point lines_position = {.x = 50, .y = 150};
    drawText(renderer, font, buffer, lines_position);

    // "Next" label
    SDL_Point next_label_position = {.x = SCREEN_WIDTH_PX - 200, .y = 100};
    drawText(renderer, font, "Next:", next_label_position);
}

void drawNextTetromino(SDL_Renderer *renderer, uint8_t next_piece[PIECE_SIZE], uint8_t color) {
    SDL_Point start_position = {.x = SCREEN_WIDTH_PX - 200, .y = 150}; // Position for "Next" tetromino

    for (int i = 0; i < PIECE_SIZE; i++) {
        if (next_piece[i]) {
            int x, y;
            getXY(i, &x, &y);

            SDL_Rect block = {
                start_position.x + x * BLOCK_SIZE_PX,
                start_position.y + y * BLOCK_SIZE_PX,
                BLOCK_SIZE_PX,
                BLOCK_SIZE_PX
            };

            setColor(renderer, color);
            SDL_RenderFillRect(renderer, &block); // Draw block
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
            SDL_RenderDrawRect(renderer, &block); // Draw border
        }
    }
}

void renderGameOver(Game *game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(game->renderer);

    // Render "Game Over" text
    SDL_Point text_position = {SCREEN_WIDTH_PX / 2, SCREEN_HEIGHT_PX / 2 - 100};
    drawCenteredText(game->renderer, game->ui_font, "Game Over", text_position);

    // Render Main Menu button
    const char *option = "Main Menu";
    SDL_Rect button_rect = {
        SCREEN_WIDTH_PX / 2 - 100,
        SCREEN_HEIGHT_PX / 2 + 50,
        200,
        50
    };

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // Button background
    SDL_RenderFillRect(game->renderer, &button_rect);

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // Button border
    SDL_RenderDrawRect(game->renderer, &button_rect);

    // Center text inside the button
    int text_width, text_height;
    TTF_SizeText(game->ui_font, option, &text_width, &text_height);
    SDL_Point button_text_position = {
        button_rect.x + (button_rect.w - text_width) / 2,
        button_rect.y + (button_rect.h - text_height) / 2
    };
    drawTextBlack(game->renderer, game->ui_font, option, button_text_position);

    SDL_RenderPresent(game->renderer);

    // Handle events to detect button clicks
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->is_running = false;
            return;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            // Check if Main Menu button was clicked
            if (x >= button_rect.x && x <= button_rect.x + button_rect.w &&
                y >= button_rect.y && y <= button_rect.y + button_rect.h) {
                game->state = MENU_STATE;
                return;
            }
        }
    }
}

void renderGameElements(Game *game, uint8_t *current_piece, SDL_Point piece_position, uint8_t *placed,
                        uint8_t color, uint64_t score, uint8_t level, uint8_t lines_cleared) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(game->renderer);

    // Draw arena background
    SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
    SDL_Rect arena = {
        (SCREEN_WIDTH_PX - ARENA_WIDTH_PX) / 2,
        (SCREEN_HEIGHT_PX - ARENA_HEIGHT_PX) / 2,
        ARENA_WIDTH_PX,
        ARENA_HEIGHT_PX
    };
    SDL_RenderFillRect(game->renderer, &arena);
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(game->renderer, &arena);

    // Draw placed blocks and current piece
    drawPlaced(placed, game->renderer);
    drawTetromino(game->renderer, current_piece, piece_position, color);

    // Render game info and next tetromino
    renderGameInfo(game->renderer, game->ui_font, score, level, lines_cleared);
    drawNextTetromino(game->renderer, game->next_piece, game->next_piece_color);

    SDL_RenderPresent(game->renderer);
}

void renderHighscores(SDL_Renderer *renderer, TTF_Font *font, HighScore *high_scores, SDL_Rect *back_button) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Render "High Scores" title
    SDL_Point title_position = {SCREEN_WIDTH_PX / 2 - 100, 50};
    drawText(renderer, font, "Highest Scores", title_position);

    // Render high scores table
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%2d. %s - %d", i + 1,
                 strlen(high_scores[i].name) > 0 ? high_scores[i].name : "---",
                 high_scores[i].score);

        SDL_Point text_position = {SCREEN_WIDTH_PX / 2 - 65, 100 + i * 30};
        drawText(renderer, font, buffer, text_position);
    }

    // Render Back button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White button background
    SDL_RenderFillRect(renderer, back_button);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
    SDL_RenderDrawRect(renderer, back_button);

    SDL_Point back_text_position = {back_button->x + 65, back_button->y + 10};
    drawTextBlack(renderer, font, "Back", back_text_position);

    SDL_RenderPresent(renderer);
}

void renderMenu(Game *game, SDL_Renderer *renderer, TTF_Font *font, MenuButton *menu_buttons, int menu_button_count) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    int total_height = menu_button_count * menu_buttons[0].rect.h + (menu_button_count - 1) * 20;
    int start_y = (SCREEN_HEIGHT_PX - total_height) / 2;

    int logo_width, logo_height;
    SDL_QueryTexture(game->logo_texture, NULL, NULL, &logo_width, &logo_height);
    SDL_Rect logo_rect = {
        (SCREEN_WIDTH_PX - logo_width) / 2,
        50,
        logo_width,
        logo_height
    };
    SDL_RenderCopy(game->renderer, game->logo_texture, NULL, &logo_rect);

    for (int i = 0; i < menu_button_count; i++) {
        menu_buttons[i].rect.x = (SCREEN_WIDTH_PX - menu_buttons[i].rect.w) / 2;

        menu_buttons[i].rect.y = start_y + i * (menu_buttons[i].rect.h + 20);

        // Draw the button background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderFillRect(renderer, &menu_buttons[i].rect);

        // Draw the button border
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
        SDL_RenderDrawRect(renderer, &menu_buttons[i].rect);

        // Center the text inside the button
        SDL_Surface *text_surface = TTF_RenderText_Solid(font, menu_buttons[i].label, (SDL_Color){0, 0, 0, 255}); // Black text
        if (!text_surface) {
            fprintf(stderr, "Text rendering failed: %s\n", TTF_GetError());
            continue;
        }
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        int text_width = text_surface->w;
        int text_height = text_surface->h;
        SDL_Rect text_rect = {
            menu_buttons[i].rect.x + (menu_buttons[i].rect.w - text_width) / 2, // Center horizontally
            menu_buttons[i].rect.y + (menu_buttons[i].rect.h - text_height) / 2, // Center vertically
            text_width,
            text_height
        };

        SDL_FreeSurface(text_surface);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        SDL_DestroyTexture(text_texture);
    }

    SDL_RenderPresent(renderer);
}

void renderControls(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect *back_button) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    const char *controls[] = {
        "Controls:",
        "A - Move Left",
        "D - Move Right",
        "S - Soft Drop",
        "R - Rotate Piece"
    };

    int num_controls = sizeof(controls) / sizeof(controls[0]);
    int y_offset = SCREEN_HEIGHT_PX / 2 - (num_controls * 30) / 2;

    for (int i = 0; i < num_controls; i++) {
        SDL_Point text_position = {SCREEN_WIDTH_PX / 2 - 100, y_offset + i * 30};
        drawText(renderer, font, controls[i], text_position);
    }

    // Render Back button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White button background
    SDL_RenderFillRect(renderer, back_button);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
    SDL_RenderDrawRect(renderer, back_button);

    SDL_Point back_text_position = {back_button->x + 65, back_button->y + 10};
    drawTextBlack(renderer, font, "Back", back_text_position);

    SDL_RenderPresent(renderer);
}