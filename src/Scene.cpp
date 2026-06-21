#include "Scene.h"
#include <iostream>
Scene::Scene(std::string const& path) {
    YAML::Node scene = YAML::LoadFile(path);

    for (auto const& obj : scene["gameObjects"]) {
        if (!obj["name"]) continue;
        create(obj);
    }
    

}

bool Scene::create(YAML::Node const& obj){  
        std::string name = obj["name"].as<std::string>();
        
        if(m_objects.find(name)!=m_objects.end()) return false;
        std::unique_ptr<GameObject> ptr;

        ptr = std::make_unique<GameObject>(obj, this);

        m_objects.try_emplace(name, std::move(ptr));
        return true;

}
bool Scene::create(std::string const& prefab, std::string const& name){
    YAML::Node obj;
    obj["prefab"]=prefab;
    obj["name"]=name;
    std::unique_ptr<GameObject> ptr;
    ptr = std::make_unique<GameObject>(obj, this);

    m_objects.try_emplace(name, std::move(ptr));
    return true;

}


