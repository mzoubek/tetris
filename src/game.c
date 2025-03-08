#include "common.h"

void Game_Init(Game *game) {

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Create SDL window
    game->window = SDL_CreateWindow(
        "Tetris",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH_PX,
        SCREEN_HEIGHT_PX,
        SDL_WINDOW_SHOWN
    );

    if (!game->window) {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Create SDL renderer
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (!game->renderer) {
        fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Load fonts
    game->ui_font = TTF_OpenFont(FONT, 24);
    if (!game->ui_font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Surface *logo_surface = IMG_Load("./resources/tetris-logo.png");
    if (!logo_surface) {
        fprintf(stderr, "Failed to load logo: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    game->logo_texture = SDL_CreateTextureFromSurface(game->renderer, logo_surface);
    SDL_FreeSurface(logo_surface);
    if (!game->logo_texture) {
        fprintf(stderr, "Failed to create texture from logo: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    // Initialize highscores count
    loadHighScores("highscores.dat", game->high_scores);

    // Initialize game state
    game->state = MENU_STATE;
    game->is_running = true;
}

void runGameLoop(Game *game, uint8_t fps, SDL_Event *event) {
    uint32_t frame_delay = 1000 / fps;
    loadHighScores("highscores.dat", game->high_scores);

    // Game variables
    uint8_t current_piece[PIECE_SIZE] = {0};
    uint8_t color = 0;
    SDL_Point piece_position = {.x = ARENA_WIDTH / 2 - 2, .y = 0};
    uint8_t placed[ARENA_SIZE] = {0};
    uint8_t level = 1;
    uint8_t lines_cleared = 0;
    uint64_t score = 0;
    uint64_t frame = 0;
    bool game_over = false;

    pickPiece(current_piece, &color);
    pickPiece(game->next_piece, &game->next_piece_color);

    // Check if the first piece collides immediately (game over)
    if (collisionCheck(placed, current_piece, piece_position)) {
        game_over = true;
    }

    while (game->is_running && !game_over) {
        uint32_t frame_start = SDL_GetTicks();

        // Handle input events
        while (SDL_PollEvent(event)) {
            handleGameInput(game, event, current_piece, &piece_position, placed, &color, &game_over);
        }

        // Game logic and rendering (same as before)
        handleGameLogic(game, current_piece, &color, &piece_position, placed, &score, &level, &lines_cleared, &frame, &game_over);
        renderGameElements(game, current_piece, piece_position, placed, color, score, level, lines_cleared);

        // Frame delay
        uint32_t frame_time = SDL_GetTicks() - frame_start;
        if (frame_delay > frame_time) {
            SDL_Delay(frame_delay - frame_time);
        }

        frame++;
    }

    if (game_over) {
        if (isHighScore(score, game->high_scores)) {
            char name[4];
            getPlayerName(game->renderer, game->ui_font, name);
            addHighScore(game->high_scores, name, score);
            saveHighScores("highscores.dat", game->high_scores);

            // Transition to high-score screen
            game->state = HIGHSCORES_STATE;
        } else {
            // Display "Game Over" message
            renderGameOver(game);
        }
    }
}

void Game_Update(Game *game, uint8_t fps) {
    SDL_Event event;

    MenuButton menu_buttons[4] = {
        {{300, 200, 250, 50}, "New Game"},
        {{300, 300, 250, 50}, "Highest Scores"},
        {{300, 400, 250, 50}, "Controls"},
        {{300, 500, 250, 50}, "Quit"}
    };

    const int menu_button_count = sizeof(menu_buttons) / sizeof(menu_buttons[0]);
    SDL_Rect back_button = {SCREEN_WIDTH_PX / 2 - 100, SCREEN_HEIGHT_PX - 100, 200, 50};

    while (game->is_running) {
        // Poll events and handle based on state
        while (SDL_PollEvent(&event)) {
            switch (game->state) {
                case MENU_STATE:
                    handleMenuInput(game, &event, menu_buttons, menu_button_count);
                    break;

                case GAME_STATE:
                    runGameLoop(game, fps, &event);
                    break;

                case HIGHSCORES_STATE:
                    handleHighscoresInput(game, &event, &back_button);
                case CONTROLS_STATE:
                    // handleMenuInput(game, &event, menu_buttons, menu_button_count);
                    handleControlsInput(game, &event, &back_button);
                    break;

                case QUIT_STATE:
                    game->is_running = false;
                    break;
            }
        }

        // Render based on game state
        switch (game->state) {
            case MENU_STATE:
                renderMenu(game, game->renderer, game->ui_font, menu_buttons, menu_button_count);
                break;

            case GAME_STATE:
                runGameLoop(game, fps, &event);
                game->state = MENU_STATE; // Return to menu after gameplay
                break;

            case HIGHSCORES_STATE:
                renderHighscores(game->renderer, game->ui_font, game->high_scores, &back_button);
                break;

            case CONTROLS_STATE:
                renderControls(game->renderer, game->ui_font, &back_button);
                break;

            case QUIT_STATE:
                break;
        }
    }
}

void Game_Quit(Game *game) {
    // Clean up resources
    if (game->ui_font) {
        TTF_CloseFont(game->ui_font);
    }

    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
    }

    if (game->window) {
        SDL_DestroyWindow(game->window);
    }

    if (game->logo_texture) {
        SDL_DestroyTexture(game->logo_texture);
    }

    TTF_Quit();
    SDL_Quit();
}
