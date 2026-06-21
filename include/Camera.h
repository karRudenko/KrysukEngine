#pragma once
#include "Component.h"
#include "BasicGeometry.h"
class Camera : public Component{
    public:

        Vector3D m_angles;
        Vector3D m_position;
        Vector3D m_dir;
        Vector3D m_up;
        virtual Matrix4x4 getCameraMatrix() =0;

};