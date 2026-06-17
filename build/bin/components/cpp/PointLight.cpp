#include "PointLight.h"
#include "GameObject.h"
#include "ComponentFactory.h"
#include <iostream>
void register_PointLight()
{ComponentFactory::init().registerComponent("PointLight", []() {return std::make_unique<PointLight>();   });}

void PointLight::update(Uint32 dT){}


void  PointLight::init(YAML::Node const& data){

    
    

}
void  PointLight::start(){}

float PointLight::getLuminocity(Triangle3D const& tri){
    Vector3D toTarget=owner->m_transform.m_position-tri.m_a;
    toTarget.normalize();
    float result=dotProduct(tri.m_N,toTarget);
    if(result<0) result=0;

    return  result;
}