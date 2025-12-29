#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_oldnames.h>
#include <iostream>
#include <time.h>
#include <cmath>
//#include <unistd.h>
#include "canvas.h"

#include <SDL3/SDL_render.h>

#include "renderer.h"
#include "parser.h"

bool Renderer::Init(int width, int height, int flags){
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("SDL3 Project", width, height, flags);

    canvas.data = {width, height, 3, new unsigned char[width * height * 3]{}};

    std::cout << "buff size: " << width * height * 3 << " :" << sizeof(canvas.data.pixels) << std::endl;
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
    const char file[] = "./resource/CharlieAndMe.bmp";
    SDL_Surface* surface = SDL_LoadBMP(file);

    if(surface == nullptr){
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    canvas.data.pixels = (unsigned char*)surface->pixels;

    time_t start = time(0);
    int frameCount = 0;

    vec2d p1 = {(double)canvas.data.w / 3, 2 * (double)canvas.data.h / 3};
    vec2d p2 = {2 * (double)canvas.data.w / 3, 2 * (double)canvas.data.h / 3};
    vec2d p3 = {(double)canvas.data.w / 2,  (double)canvas.data.h / 3};
    vec2d centre = {(double)canvas.data.w, (double)canvas.data.h /2};
    
    canvas.GetBarycentricCoords(p1, p2, p3, centre);

    std::vector<vec3d> verticies;
    std::vector<int> indicies;
    std::vector<vec3d> normals;
    parseObj3d("./resource/newell_teaset/teapot.obj", verticies, indicies, normals);
    std::cout << "Teapot parsed" << std::endl;

    for(size_t i = 0; i < verticies.size(); i++){
        verticies[i] = {(verticies[i] .x * 200), (480 - verticies[i].y * 200), (verticies[i].z + 4)};
    }

    std::cout << "Points Affected" << std::endl;
    //std::cout << "Max index - " << *std::max_element(indicies.begin(), indicies.end()) << std::endl;
    std::cout << "Points size - " << verticies.size() << std::endl;

    for(size_t i = 0; i < indicies.size(); i += 3) {
        //std::cout << "Indicies - " << i << ", " << i + 1 << ", " << i + 2 << std::endl;
        p1 = {(verticies[indicies[i] - 1].x) / (verticies[indicies[i] - 1].z) + 200, ((verticies[indicies[i] - 1].y)) / (verticies[indicies[i] - 1].z) + 200};
        p2 = {(verticies[indicies[i + 1] - 1].x) / (verticies[indicies[i + 1] - 1].z) + 200, ((verticies[indicies[i + 1] - 1].y)) / (verticies[indicies[i + 1] - 1].z) + 200};
        p3 = {(verticies[indicies[i + 2] - 1].x) / (verticies[indicies[i + 2] - 1].z) + 200, ((verticies[indicies[i + 2] - 1].y)) / (verticies[indicies[i + 2] - 1].z) + 200};

        if(Vec2Winding(p1, p2, p3) <= 0) continue;
        vec3d normVert = Vec3Norm(verticies[indicies[i] - 1]);
        //normVert = {-normVert.x, -normVert.y, -normVert.z};
        //std::cout << "Normal Verticies - " << normVert.x << ", " << normVert.y << ", " << normVert.z << std::endl;
        centre.x = std::max(0.0, Vec3DotProd(normVert, normals[indicies[i] - 1]));

        canvas.GetBarycentricCoords(p1, p2, p3, centre);
    }

    std::cout << "Teapot Drawn" << std::endl;

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
