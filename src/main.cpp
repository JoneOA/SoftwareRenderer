#include "renderer.h"

#define WIDTH 640
#define HEIGHT 480

SDL_Window* window = NULL;  

int main() {

    Renderer renderer;
    renderer.Init(WIDTH, HEIGHT, 0);
    renderer.Run();

    return 0;
}

