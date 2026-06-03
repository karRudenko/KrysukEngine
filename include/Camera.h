#pragma once
#include "Component.h"
#include "BasicGeometry.h"
class Camera : public Component{
    public:

        Vector3D m_angles;
        Vector3D m_position;
        virtual Matrix4x4 getCameraMatrix() const =0;

};