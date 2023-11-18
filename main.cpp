#include <iostream>
#include <ranges>
#include <SDL.h>

int helper(double x, double y, double cx, double cy, int acc) {
    if (acc > 100) {
        return acc;
    } else if (x*x > 4 || y*y > 4) {
        return acc;
    } else {
        return helper(x*x-y*y+cx, 2*x*y+cy, cx, cy, acc + 1);
    }
}

int mandelbrot(double x, double y) {
    return helper(x, y, x, y, 0);
}

void drawPixel(SDL_Renderer *renderer, int x, int y) {
    double coordX, coordY;
    coordX = (x - 300.0) / 150;
    coordY = (200.0 - y) / 150;
    auto value = mandelbrot(coordX, coordY);
    SDL_SetRenderDrawColor(renderer, value*2.55, value*2.55, 0, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (auto i : std::views::iota(0, 600)) {
        for (auto j : std::views::iota(0, 400)) {
            drawPixel(renderer, i, j);
        }
    }

    SDL_RenderPresent(renderer);

    // Wait for the user to close the window
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
