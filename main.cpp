#include <iostream>
#include <ranges>
#include <SDL.h>

static int helper(double x, double y, double cx, double cy, int acc) {
    if (acc > 99) {
        return acc;
    } else if (x*x > 4 || y*y > 4) {
        return acc;
    } else {
        return helper(x*x-y*y+cx, 2*x*y+cy, cx, cy, acc + 1);
    }
}

static int mandelbrot(double x, double y) {
    return helper(x, y, x, y, 0);
}

static void drawPixel(SDL_Renderer *renderer, int x, int y, double scale, double originX, double originY) {
    auto value = mandelbrot(
        x * scale + originX,
        y * scale + originY
    );
    SDL_SetRenderDrawColor(renderer, value*2.55, value*2.55, 0, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

static void draw(SDL_Renderer *renderer, int width, int height, double scale, double originX, double originY) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (auto i : std::views::iota(0, width)) {
        for (auto j : std::views::iota(0, height)) {
            drawPixel(renderer, i, j, scale, originX, originY);
        }
    }

    SDL_RenderPresent(renderer);
}

int main(void) {
    int width = 640;
    int height = 480;
    double viewHeight = 3.0;
    double originX = -2.5;
    double originY = -viewHeight / 2;
    double scale = viewHeight / height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Mandelbrot",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
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


    SDL_Event e;
    bool quit = false;
    bool redraw = true;
    while (!quit) {
        if (redraw) {
            draw(renderer, width, height, scale, originX, originY);
            redraw = false;
        }
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: {
                    quit = true;
                    break;
                }
                case SDL_WINDOWEVENT: {
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        width = e.window.data1;
                        height = e.window.data2;
                        scale = viewHeight / height;
                        redraw = true;
                    }
                    break;
                }
                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_PLUS: {
                            viewHeight /= 1.1;
                            scale = viewHeight / height;
                            redraw = true;
                            break;
                        }
                        case SDLK_MINUS: {
                            viewHeight *= 1.1;
                            scale = viewHeight / height;
                            redraw = true;
                            break;
                        }
                        case SDLK_LEFT: {
                            originX -= viewHeight / 10;
                            redraw = true;
                            break;
                        }
                        case SDLK_RIGHT: {
                            originX += viewHeight / 10;
                            redraw = true;
                            break;
                        }
                        case SDLK_UP: {
                            originY -= viewHeight / 10;
                            redraw = true;
                            break;
                        }
                        case SDLK_DOWN: {
                            originY += viewHeight / 10;
                            redraw = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
