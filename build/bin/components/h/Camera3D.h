#pragma once
#include "Camera.h"
class Camera3D : public Camera{
    public:
        Vector3D m_angles;
        Vector3D m_position;
        virtual void start() override;
        virtual void update(Uint32 dT) override;
        virtual void init(YAML::Node const& data) override;
        virtual Matrix4x4 getCameraMatrix() const override;
};
