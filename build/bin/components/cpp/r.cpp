#include "r.h"
#include "GameObject.h"
#include "ComponentFactory.h"
#include <cmath>
void register_r()
{ComponentFactory::init().registerComponent("r", []() {return std::make_unique<r>();   });}


void r::update(Uint32 dT)
{   
    owner->m_transform.m_rotation=owner->m_transform.m_rotation+Vector3D(0,10,0);
}



 void r::start(){};
 void r::init(YAML::Node const& data){};