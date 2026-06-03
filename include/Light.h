#pragma once
#include "Component.h"
#include "BasicGeometry.h"
class Light : public Component{
    public:

        virtual float getLuminocity(Triangle3D const& tri) = 0;
};