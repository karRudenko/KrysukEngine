#include "GameObject.h"
#include <iostream>
#include "Scene.h"
GameObject::GameObject(YAML::Node const& obj,Scene* scene) : m_transform(Transform(obj)), m_scene(scene), m_name(){
    
    m_name=obj["name"].as<std::string>();

        try {
            
            
            if(obj["prefab"]){
                std::string prefabPath = obj["prefab"].as<std::string>();
                YAML::Node prefab = YAML::LoadFile(prefabPath);
                if(prefab["components"]){
                    for (auto const& comp : prefab["components"]) {
                        if (comp["name"] && comp["type"]){
                                
                            addComponent(comp);
                        }
                    }
                }
            }





            } catch (const std::exception& e) {
                std::cerr << "Failed to load prefab: " << e.what() << std::endl;
            }

    
}














void GameObject::update(Uint32 dt){
    for(auto& c:m_components){
        c.second->update(dt);
    }
}




void GameObject::addComponent(YAML::Node const& data){
    auto comp = ComponentFactory::init().create(data["name"].as<std::string>());

    if(!comp) return;
    comp->owner=this;
    comp->init(data);
    comp->start();
    m_components.insert({data["type"].as<std::string>(),std::move(comp)});


}









void GameObject::addComponent(std::string const& name, YAML::Node const& data){
    auto comp = ComponentFactory::init().create(name);


    if(!comp) return;
    comp->owner=this;
    comp->init(data);
    comp->start();
    m_components.insert({name,std::move(comp)});


}

void GameObject::addComponent(std::string const& name){
    auto comp = ComponentFactory::init().create(name);
    
    if(!comp) return;
    comp->owner=this;
    m_components.insert({name,std::move(comp)});


}





