#ifndef ENGINE_H
#define ENGINE_H

#include "Renderer.h"

class Engine{
    public:
        Engine();
        void start();


    private:
        Settings m_settings;
        Window m_window;
        Timer m_timer;
        Renderer m_renderer;
        SDL_Event m_event;
        Scene m_scene;
        bool m_quit=false;
};

#endif