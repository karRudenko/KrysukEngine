#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <unordered_set>
class  InputHandler{
    public:
        void static pullEvents();
        bool  static keyEventofType(SDL_EventType key);
        bool  static keyIsPressed(SDL_KeyCode key);

    private:
        static SDL_Event m_e;
        static std::vector<SDL_Event> all_events;
        static std::unordered_set<SDL_Keycode> m_keysPressed;

};