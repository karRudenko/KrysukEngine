#include "Camera3D.h"
#include "GameObject.h"
#include "ComponentFactory.h"
#include <iostream>
void register_Camera3D()
{ComponentFactory::init().registerComponent("Camera3D", []() {return std::make_unique<Camera3D>();   });}

void Camera3D::update(Uint32 dT){
    m_angles=owner->m_transform.m_rotation;
    m_position=owner->m_transform.m_position;
}
Matrix4x4 Camera3D::getCameraMatrix() const{
    Vector3D up={0,1,0};
    Vector3D forward={0,0,1};

    forward.rotate(m_angles);
    forward=forward+m_position;
    return quickInverse(getPointAtMatrix(m_position,forward, up));
}

void  Camera3D::init(YAML::Node const& data){}
void Camera3D::start(){}