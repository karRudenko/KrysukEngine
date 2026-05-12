#ifndef SCENE_H
#define SCENE_H
#include <unordered_map>
#include <string>
#include "GameObject.h"
#include <yaml-cpp/yaml.h>

class Scene{
    public:
        Scene() =default;
        Scene(std::string const& path);
        bool create(std::string const& prefab, std::string const& name);
        bool create(YAML::Node const& obj);




        std::string m_sceneName;
        std::unordered_map<std::string, std::unique_ptr<GameObject>> m_objects;
    
        template<typename T>
        std::vector<T*> findComponents(std::string const& name) const{
            std::vector<T*> result;
            for(auto& [_,obj]: m_objects){
                T* ptr=obj->findComponent<T>(name);
                if(ptr){
                    result.push_back(ptr);
                }
            }
            return result;
        }




    
};



#endif