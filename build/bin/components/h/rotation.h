#pragma once
#include "Component.h"
class rotation : public Component{
    public:
        virtual ~rotation() =default;
        virtual void update(Uint32 dT) override;
        virtual void start() override;
        virtual void init(YAML::Node const& data) override;
};
