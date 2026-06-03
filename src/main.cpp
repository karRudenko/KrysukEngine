#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include "Engine.h"
#include "BasicGeometry.h"

int main(){
    SDL_Init(SDL_INIT_EVERYTHING);
    Engine test;

    
    test.start();
    
    SDL_Quit();

    return 0;
}