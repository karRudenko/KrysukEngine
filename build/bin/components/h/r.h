#pragma once
#include "Component.h"
class r : public Component{
    public:
        virtual ~r() =default;
        virtual void update(Uint32 dT) override;
        virtual void start() override;
        virtual void init(YAML::Node const& data) override;
};
