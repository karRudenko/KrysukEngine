#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>
class GameObject;


class Component{
    public:
        virtual ~Component() = default;
        virtual void update(Uint32 dT) = 0;
        virtual void start() = 0;
        virtual void init(YAML::Node const& data);
        GameObject* owner; 
         
       
};
#endif