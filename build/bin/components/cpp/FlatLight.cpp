#include "FlatLight.h"
#include "GameObject.h"
#include "ComponentFactory.h"
#include <iostream>
void register_FlatLight()
{ComponentFactory::init().registerComponent("FlatLight", []() {return std::make_unique<FlatLight>();   });}

void FlatLight::update(Uint32 dT){}


void  FlatLight::init(YAML::Node const& data){
    std::vector<float> f=data["data"].as<std::vector<float>>();
    m_lightDir=Vector3D(f[0],f[1],f[2]);
    m_lightDir.normalize();
    
    

}
void  FlatLight::start(){}

float FlatLight::getLuminocity(Triangle3D const& tri){

    float result=dotProduct(tri.m_N,m_lightDir);
    if(result<0) result=0;

    return  result;
}