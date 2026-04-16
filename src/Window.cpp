
#include "Window.h"


Window::Window(Settings const& settings){
    height=settings.m_windowHeight;
    width=settings.m_windowWidth;
    gWindow=NULL;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        std::cout<<"fail to init "<< SDL_GetError()<<std::endl;

    }
    else{
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        gWindow  = SDL_CreateWindow( settings.m_windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FOREIGN | SDL_WINDOW_OPENGL ); //SDL_WINDOW_FULLSCREEN_DESKTOP
        if( gWindow == NULL ){
            std::cout<<"Window could not be created! SDL_Error:" << SDL_GetError() <<std::endl;
        }
    }
    
}
Window::~Window(){
    SDL_DestroyWindow(gWindow);
    gWindow=nullptr;
    
}


bool init(Window* window){
    bool success=true;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        std::cout<<"fail to init "<< SDL_GetError()<<std::endl;
        success = false;

    }
    else{
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        window->gWindow  = SDL_CreateWindow( "EVA Terminal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window->width, window->height, SDL_WINDOW_FULLSCREEN_DESKTOP );
        if( window->gWindow == NULL ){
            std::cout<<"Window could not be created! SDL_Error:" << SDL_GetError() <<std::endl;
            success = false;
        }
        else{

        }
    }
    return success;
}






void Window::quitWindow(){
    quit = true;
}




Timer::Timer(){
    started=false;
    paused=false;
    startTicks=0;
    pausedTicks=0;
}

void Timer::start(){
    started=true;
    startTicks=SDL_GetTicks();
    pausedTicks=0;
}

void Timer::stop(){
    started=false;
    paused=false;
    startTicks=0;
    pausedTicks=0;
}


void Timer::pause(){
    if(!paused && started){
        paused=true;


        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks=0;
    }
}


void Timer::unpause(){
    if(started &&  paused){
        paused=false;

        startTicks=SDL_GetTicks()-pausedTicks;
        pausedTicks=0;

    }

}



bool Timer::isPaused(){
    return paused;
}


bool Timer::isStarted(){
    return started;
}


Uint32 Timer::getTicks(){
    if(started){
        if(paused){
            return pausedTicks;
        }
        else{
            return SDL_GetTicks()-startTicks;
        }
    }
    return 0;
}