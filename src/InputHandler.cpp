#include "InputHandler.h"
#include <iostream>
SDL_Event InputHandler::m_e;
std::vector<SDL_Event> InputHandler::all_events={};
std::unordered_set<SDL_Keycode> InputHandler::m_keysPressed={};
void InputHandler::pullEvents(){
    all_events.clear();
    while(SDL_PollEvent(&m_e)){
        all_events.push_back(m_e);
        switch(m_e.type){
            case SDL_KEYDOWN:
                m_keysPressed.insert(m_e.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_keysPressed.erase(m_e.key.keysym.sym);
                break;

        }
	}

};


bool InputHandler::keyEventofType(SDL_EventType key){
    for(auto& e : all_events){
        if(e.type==key) return true;
    }
    return false;
}


bool InputHandler::keyIsPressed(SDL_KeyCode key){
    return m_keysPressed.count(key);
}