#pragma once 

#include <SDL3/SDL.h>

#include "canvas.h"

class Renderer {
public:
    bool Init(int width, int height, int flags);
    bool Run();

private:
    SDL_Window* win;
    SDL_Renderer* ren;
    Canvas* canvas;

    int increment = 1;
};
