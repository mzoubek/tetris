#include "common.h"

void saveHighScores(const char *filename, HighScore *high_scores) {
    FILE *file = fopen(filename, "wb");
    if (file) {
        size_t written_count = fwrite(high_scores, sizeof(HighScore), MAX_HIGH_SCORES, file);
        fclose(file);
        if (written_count != MAX_HIGH_SCORES) {
            fprintf(stderr, "Failed to write all high scores to file: %s\n", filename);
        }
    } else {
        fprintf(stderr, "Failed to open high scores file for writing: %s\n", filename);
    }
}

void loadHighScores(const char *filename, HighScore *high_scores) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        size_t read_count = fread(high_scores, sizeof(HighScore), MAX_HIGH_SCORES, file);
        fclose(file);

        // Validate the read count
        if (read_count != MAX_HIGH_SCORES) {
            // Reinitialize to default if the file is corrupted
            for (int i = 0; i < MAX_HIGH_SCORES; i++) {
                strcpy(high_scores[i].name, "---");
                high_scores[i].score = 0;
            }
            saveHighScores(filename, high_scores);
        }
    } else {
        // File doesn't exist, initialize with default values
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            strcpy(high_scores[i].name, "---");
            high_scores[i].score = 0;
        }
        saveHighScores(filename, high_scores);
    }
}

void addHighScore(HighScore *high_scores, const char *name, int score) {
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (score > high_scores[i].score) {
            // Shift lower scores down
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--) {
                high_scores[j] = high_scores[j - 1];
            }
            // Insert new high score
            strncpy(high_scores[i].name, name, 3); // Copy only the first 3 characters
            high_scores[i].name[3] = '\0';         // Null-terminate the string
            high_scores[i].score = score;
            break;
        }
    }
}

bool isHighScore(int score, HighScore *high_scores) {
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (score > high_scores[i].score) {
            return true; // The score qualifies for the top 10
        }
    }
    return false; // The score does not qualify
}

void getPlayerName(SDL_Renderer *renderer, TTF_Font *font, char *name) {

    SDL_Event event;
    bool done = false;
    int char_count = 0;

    // Clear the name buffer
    memset(name, '\0', 4);

    while (!done) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Point prompt_pos = {SCREEN_WIDTH_PX / 2 - 100, SCREEN_HEIGHT_PX / 2 - 50};
        drawText(renderer, font, "Enter Name:", prompt_pos);

        SDL_Point name_pos = {SCREEN_WIDTH_PX / 2 - 50, SCREEN_HEIGHT_PX / 2};
        drawText(renderer, font, name, name_pos);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                exit(0);
            } else if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;

                if (key >= SDLK_a && key <= SDLK_z && char_count < 3) {
                    name[char_count++] = key;
                    name[char_count] = '\0'; // Null-terminate
                } else if (key == SDLK_BACKSPACE && char_count > 0) {
                    name[--char_count] = '\0';
                } else if (key == SDLK_RETURN || key == SDLK_RETURN2) {
                    done = true;
                }
            }
        }
    }
}
