#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_oldnames.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include "canvas.h"

#include <SDL3/SDL_render.h>

#include "renderer.h"

bool Renderer::Init(int width, int height, int flags){
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("SDL3 Project", width, height, flags);
    
    canvas.data = {width, height, 3, new char[width * height * 3]{}};

    
    std::cout << "buff size: " <<width * height * 3 << " :" << sizeof(canvas.data.pixels) << std::endl;
    if(win == nullptr){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    ren = SDL_CreateRenderer(win, NULL);
    if(ren == nullptr){
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }
    
    SDL_RenderPresent(ren);
    return true;
}

bool Renderer::Run(){


    SDL_Event e;
    bool quit = false;
    const char file[] = "/home/owainj/dev/Cpp/SoftwareRenderer/resource/CharlieAndMe.bmp";
    SDL_Surface* surface = SDL_LoadBMP(file);
    
    canvas.data.pixels = (char*)surface->pixels;

    if(surface == nullptr){
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    time_t start = time(0);
    int frameCount = 0;

    vec2d p3 = {(double)canvas.data.w / 2,  (double)canvas.data.h / 3};
    vec2d p1 = {(double)canvas.data.w / 3, 2 * (double)canvas.data.h / 3};
    vec2d p2 = {2 * (double)canvas.data.w / 3, 2 * (double)canvas.data.h / 3};

    std::cout << "P1 " << p1.x << ", " << p1.y << std::endl;
    std::cout << "P2 " << p2.x << ", " << p2.y << std::endl;
    std::cout << "P3 " << p3.x << ", " << p3.y << std::endl;
    
    canvas.GetBarycentricCoords(p1, p2, p3, p1);
    canvas.DrawTriangle(p1, p2 , p3);
    while(!quit){
        while(SDL_PollEvent(&e)!= 0)
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }

        SDL_RenderClear(ren);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
        SDL_RenderTexture(ren, texture, nullptr, nullptr);
        SDL_RenderPresent(ren);

        SDL_DestroyTexture(texture);
        frameCount++;
        if(time(0) - start >= 1)
        {
            std::cout << (frameCount) << std::endl;
            start = time(0);
            frameCount = 0;
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return true;
}
