#include "rotation.h"
#include "GameObject.h"
#include "ComponentFactory.h"
#include "InputHandler.h"
void register_rotation()
{ComponentFactory::init().registerComponent("rotation", []() {return std::make_unique<rotation>();   });}


void rotation::update(Uint32 dT)
{
    float moveSpeed = 0.1f;
    float rotSpeed  = 1;
    Vector3D dir={0,0,1};
    dir.rotate(owner->m_transform.m_rotation);
    if (InputHandler::keyIsPressed(SDLK_w)) {
        owner->m_transform.m_position = owner->m_transform.m_position + dir * moveSpeed;
    }

    if (InputHandler::keyIsPressed(SDLK_s)) {
        owner->m_transform.m_position = owner->m_transform.m_position - dir * moveSpeed;
    }


    if (InputHandler::keyIsPressed(SDLK_UP)) {
        owner->m_transform.m_rotation.m_x += rotSpeed;
    }

    if (InputHandler::keyIsPressed(SDLK_DOWN)) {
        owner->m_transform.m_rotation.m_x -= rotSpeed;
    }

    if (InputHandler::keyIsPressed(SDLK_LEFT)) {
        owner->m_transform.m_rotation.m_y += rotSpeed;
    }

    if (InputHandler::keyIsPressed(SDLK_RIGHT)) {
        owner->m_transform.m_rotation.m_y -= rotSpeed;
    }
}
 void rotation::start(){};
 void rotation::init(YAML::Node const& data){};