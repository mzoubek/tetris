// #include <SDL.h>
// #include <SDL_timer.h>
// #include <SDL_image.h>
// #include <stdio.h>

// int main(void) {
//     if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
//         printf("error initializing SDL: %s\n", SDL_GetError());
//         return 1;
//     }

//     SDL_Window *win = SDL_CreateWindow("Tetris",
//                                         SDL_WINDOWPOS_CENTERED,
//                                         SDL_WINDOWPOS_CENTERED,
//                                         640, 400, 0);

//     if (!win) {
//         printf("error creating window: %s\n", SDL_GetError());
//         SDL_Quit();
//         return 1;
//     }

//     Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
//     SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
//     if (!rend) {
//         printf("error creating renderer: %s\n", SDL_GetError());
//         SDL_DestroyWindow(win);
//         SDL_Quit();
//         return 1;
//     }

//     SDL_Surface* surface = IMG_Load("resources/hello.png");
//     if (!surface) {
//         printf("error creating surface\n");
//         SDL_DestroyRenderer(rend);
//         SDL_DestroyWindow(win);
//         SDL_Quit();
//         return 1;
//     }

//     SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
//     SDL_FreeSurface(surface);
//     if (!tex) {
//         printf("error creating texture: %s\n", SDL_GetError());
//         SDL_DestroyRenderer(rend);
//         SDL_DestroyWindow(win);
//         SDL_Quit();
//         return 1;
//     }

//     SDL_RenderClear(rend);

//     SDL_RenderCopy(rend, tex, NULL, NULL);
//     SDL_RenderPresent(rend);

//     SDL_Delay(5000);

//     SDL_DestroyTexture(tex);
//     SDL_DestroyRenderer(rend);
//     SDL_DestroyWindow(win);
//     SDL_Quit();
//     return 0;
// }

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

    SDL_Window *win = SDL_CreateWindow("Tetris",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 400,
                                       SDL_WINDOW_ALLOW_HIGHDPI);
    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rend) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_RenderClear(rend);
        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
