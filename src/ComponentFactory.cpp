#include "ComponentFactory.h"
#include <iostream>

ComponentFactory& ComponentFactory::init(){
    static ComponentFactory f;
    return f;
}



bool ComponentFactory::registerComponent(std::string const& name, Creator creator){
    if(m_creators.find(name)!=m_creators.end()) return false;

    m_creators[name]=creator;
    return true;    

}



std::unique_ptr<Component> ComponentFactory::create(std::string const& name){
    if(m_creators.find(name)==m_creators.end()) return nullptr;
    return m_creators[name]();
}