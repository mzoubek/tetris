#include "common.h"

int main(void) {
    Game game;
    Game_Init(&game);
    Game_Update(&game, 60); // Run the game loop at 60 FPS
    Game_Quit(&game);
    return 0;
}
