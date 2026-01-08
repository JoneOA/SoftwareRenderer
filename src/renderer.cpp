#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <cmath>
//#include <unistd.h>
#include "canvas.h"

#include <SDL3/SDL_render.h>

#include "renderer.h"
#include "parser.h"

bool Renderer::Init(int width, int height, int flags){
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("SDL3 Project", width, height, flags);

    canvas = new Canvas(width, height, 3, new unsigned char[width * height * 3]{});

    std::cout << "buff size: " << width * height * 3 << " :" << sizeof(canvas->data.pixels) << std::endl;
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

    canvas->data.pixels = (unsigned char*)surface->pixels;
    Canvas blank = *canvas;

    time_t start = time(0);
    int frameCount = 0;

    vec2d p1 = {(double)canvas->data.w / 3, 2 * (double)canvas->data.h / 3};
    vec3c c1 = {0xcb, 0xC0, 0xff};
    vec2d p2 = {2 * (double)canvas->data.w / 3, 2 * (double)canvas->data.h / 3};
    vec3c c2 = {0xcb, 0xC0, 0xff};
    vec2d p3 = {(double)canvas->data.w / 2,  (double)canvas->data.h / 3};
    vec3c c3 = {0xcb, 0xC0, 0xff};
    vec2d centre = {(double)canvas->data.w, (double)canvas->data.h /2};

    std::cout << "Tri vec winding : " << Vec2Winding(p1, p2, p3) << std::endl;
    //canvas.GetBarycentricCoords(p1, p2, p3, c1, c2, c3);

    std::vector<vec3d> verticies;
    std::vector<int> indicies;
    std::vector<vec3d> normals;
    parseObj3d("./resource/utah_teapot.obj", verticies, indicies, normals);
    std::cout << "Teapot parsed" << std::endl;

    vec3d scale  = {1, 1, 1};
    vec3d shift = {0, -2, 5};
    vec3d shiftR = {-shift.x, -shift.y, -shift.z};

    for(size_t i = 0; i < verticies.size(); i++)
    {
        verticies[i] = Vec3Scale(verticies[i], scale);
        verticies[i] = Vec3Sum(verticies[i], shift);
    }

    std::cout << std::max_element(verticies.begin(), verticies.end(), [](const auto& a, const auto& b){ return a.x < b.x;})->x << ", " << std::min_element(verticies.begin(), verticies.end(), [](const auto& a, const auto& b){ return a.x < b.x;})->x << std::endl;
    std::cout << std::max_element(verticies.begin(), verticies.end(), [](const auto& a, const auto& b){ return a.y < b.y;})->y << ", " << std::min_element(verticies.begin(), verticies.end(), [](const auto& a, const auto& b){ return a.y < b.y;})->y << std::endl;
    std::cout << std::max_element(verticies.begin(), verticies.end(), [](const auto& a, const auto& b){ return a.z < b.z;})->z << ", " << std::min_element(verticies.begin(), verticies.end(), [](const auto& a, const auto& b){ return a.z < b.z;})->z << std::endl;

    std::cout << "Points Affected" << std::endl;
    std::cout << "Index Size -  " << indicies.size() << std::endl;
    std::cout << "Max Index -  " << *std::max_element(indicies.begin(), indicies.end()) << std::endl;
    std::cout << "Points size - " << verticies.size() << std::endl;

    for(size_t i = 0; i < indicies.size(); i += 3) {
        canvas->DrawTriangle3D(verticies[indicies[i] - 1], verticies[indicies[i + 1] - 1], verticies[indicies[i + 2] - 1], c1, c2, c3, normals[indicies[i] - 1], normals[indicies[i + 1] - 1], normals[indicies[i + 2] - 1]);
    }

    //for(size_t i = 0; i < indicies.size(); i += 3) {
    //    canvas->DrawTriangle3D(verticies[indicies[i] - 1], verticies[indicies[i + 1] - 1], verticies[indicies[i + 2] - 1], c1, c2, c3, normals[indicies[i] - 1], normals[indicies[i + 1] - 1], normals[indicies[i + 2] - 1]);
    //}

    std::cout << "Teapot Drawn" << std::endl;
    //for(int i = 0; i < canvas->data.w * canvas->data.h * canvas->data.pitch; i++)
    //{
    //    canvas->data.pixels[i] = (unsigned char)std::floor(canvas->m_depth.pixels[i]);
    //}
    

    double angle = 2*M_PI;
    int rotationCounter = 0;

    while(!quit){
        while(SDL_PollEvent(&e)!= 0)
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }
        
        surface = SDL_LoadBMP(file);
        *canvas = blank;
        canvas->data.pixels = (unsigned char*)surface->pixels;

        for(size_t i = 0; i < verticies.size(); i++)
        {
            verticies[i] = Vec3Sum(verticies[i], shiftR);
            verticies[i].x = verticies[i].x * cos(double(rotationCounter) / 5000 * angle) - verticies[i].z * sin(double(rotationCounter) / 5000 * angle);
            verticies[i].z = verticies[i].x * sin(double(rotationCounter) / 5000 * angle) + verticies[i].z * cos(double(rotationCounter) / 5000 * angle);
            normals[i].x = normals[i].x * cos(double(rotationCounter) / 5000 * angle) - normals[i].z * sin(double(rotationCounter) / 5000 * angle);
            normals[i].z = normals[i].x * sin(double(rotationCounter) / 5000 * angle) + normals[i].z * cos(double(rotationCounter) / 5000 * angle);
            verticies[i] = Vec3Sum(verticies[i], shift);
        }

        for(size_t i = 0; i < indicies.size(); i += 3) {
            canvas->DrawTriangle3D(verticies[indicies[i] - 1], verticies[indicies[i + 1] - 1], verticies[indicies[i + 2] - 1], c1, c2, c3, normals[indicies[i] - 1], normals[indicies[i + 1] - 1], normals[indicies[i + 2] - 1]);
        }

        rotationCounter++;
        rotationCounter %= int(angle);
        canvas->ClearDepthBuffer();

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
